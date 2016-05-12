#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <time.h>

int randomGenerator() {
    int ranNum;
    srand(time(NULL));
    ranNum = (rand() % 900) + 100;
    return ranNum;
}

int main(int argc, char** argv)
{
    int fd,fd2;
    int i,j;

    int* ptr;
    int* ptr2;

    sem_t* startSem;
    sem_t* twoClientSem;
    sem_t* aSem;
    sem_t* bSem;
    sem_t* clientInputSem;
    sem_t* waitServerPrintSem;
    sem_t* allReadyToStartSem;
    sem_t* finishSem;

    int semVal;

    int zero = 0;
    int temp;

    int ranNum;
    int random[3];

    int userNum;
    int user[3];

    int check[3];

    int strike;
    int ball;

    fd = open("input.txt", O_RDWR|O_CREAT, S_IRWXU);
    write(fd, &zero, sizeof(int));
    ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    fd2 = open("output.txt", O_RDWR|O_CREAT, S_IRWXU);
    write(fd2, &zero, sizeof(int));
    ptr2 = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);
    close(fd2);

    sem_unlink("startSem");
    if((startSem = sem_open("startSem", O_CREAT, 0644, 0)) == SEM_FAILED) {
	perror("Sem Failed");
	exit(1);
    }

    sem_unlink("twoClientSem");
    if((twoClientSem = sem_open("twoClientSem", O_CREAT, 0644, 2)) == SEM_FAILED) {
	perror("Sem Failed");
	exit(1);
    }

    sem_unlink("aSem");
    if((aSem = sem_open("aSem", O_CREAT, 0644, 1)) == SEM_FAILED) {
	perror("Sem Failed");
	exit(1);
    }

    sem_unlink("bSem");
    if((bSem = sem_open("bSem", O_CREAT, 0644, 0)) == SEM_FAILED) {
	perror("Sem Failed");
	exit(1);
    }
    
    sem_unlink("finishSem");
    if((finishSem = sem_open("finishSem", O_CREAT, 0644, 0)) == SEM_FAILED) {
	perror("Sem Failed");
	exit(1);
    }

    sem_unlink("clientInputSem");
    if((clientInputSem = sem_open("clientInputSem", O_CREAT, 0644, 0)) == SEM_FAILED) {
	perror("Sem Failed");
	exit(1);
    }

    sem_unlink("waitServerPrintSem");
    if((waitServerPrintSem = sem_open("waitServerPrintSem", O_CREAT, 0644, 0)) == SEM_FAILED) {
	perror("Sem Failed");
	exit(1);
    }

    sem_unlink("allReadyToStartSem");
    if((allReadyToStartSem = sem_open("allReadyToStartSem", O_CREAT, 0644, 0)) == SEM_FAILED) {
	perror("Sem Failed");
	exit(1);
    }

    /*
       sem_getvalue(startSem, &semVal);
       printf("startSem : %d\n",semVal);
     */

    ranNum = randomGenerator();
    temp = ranNum;
    i=2;
    while(ranNum > 0) {
	random[i--] = ranNum % 10;
	ranNum /= 10;
    }
    ranNum = temp;
    printf("Random Number : ");
    for(i=0; i<3; i++) {
	printf("%d",random[i]);
    }
    puts("");


    sem_wait(startSem);
    printf("Client hi\n");


    /*
       sem_getvalue(startSem, &semVal);
       printf("startSem : %d\n",semVal);
     */

    sem_wait(startSem);

    printf("Client hi\n");
    /*
       sem_getvalue(startSem, &semVal);
       printf("startSem : %d\n",semVal);
     */

    /* Starting game */
    printf("=========================\n");
    printf("        Game Start       \n");
    printf("=========================\n");

    sem_post(allReadyToStartSem);
    sem_post(allReadyToStartSem);

    while(1) {

	sem_wait(clientInputSem);

	userNum = ptr[0];
	temp = userNum;
	i=2;
	while(userNum > 0) {
	    user[i--] = userNum % 10;
	    userNum /= 10;
	}
	userNum = temp;
	strike = 0;
	ball = 0;
	memset(check,0,sizeof(check));

	for(i=0; i<3; i++) {
	    if(random[i] == user[i]) {
		strike++;
		check[i]++;
	    }
	}

	if(strike == 3) {
	    ptr2[0] = 3;
	    ptr2[1] = 0;
	} else {
	    for(i=0; i<3; i++) {
		if(check[i]) continue;
		for(j=0; j<3; j++) {
		    if(check[j]) continue;
		    if(user[i] == random[j]) {
			ball++;
			break;
		    }
		}
	    }
	    ptr2[0] = strike;
	    ptr2[1] = ball;
	}
	ptr2[2] = ptr[1]; // save pid
	ptr2[3] = ptr[0]; // save userNum
	if(strike == 3) {
	    ptr2[4] = ptr[1]; // save winner pid
	}
	printf("[%d] %d : %d strike, %d ball\n",ptr2[2],ptr2[3],strike,ball);

	sem_post(waitServerPrintSem);
	if(strike == 3) {
	    sem_wait(finishSem);
	    break;
	}
    }

    printf("\n%d win!!\n",ptr2[4]);

    sem_close(startSem);
    exit(0);
}
