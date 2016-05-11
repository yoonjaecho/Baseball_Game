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
    sem_t* clientNumSem;
    sem_t* inputNumSem;
    sem_t* waitClientSem;
    sem_t* waitServerPrintSem;

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

    sem_unlink("clientNumSem");
    if((clientNumSem = sem_open("clientNumSem", O_CREAT, 0644, 2)) == SEM_FAILED) {
	perror("Sem Failed");
	exit(1);
    }

    sem_unlink("inputNumSem");
    if((inputNumSem = sem_open("inputNumSem", O_CREAT, 0644, 1)) == SEM_FAILED) {
	perror("Sem Failed");
	exit(1);
    }

    sem_unlink("waitClientSem");
    if((waitClientSem = sem_open("waitClientSem", O_CREAT, 0644, 0)) == SEM_FAILED) {
	perror("Sem Failed");
	exit(1);
    }

    sem_unlink("waitServerPrintSem");
    if((waitServerPrintSem = sem_open("waitServerPrintSem", O_CREAT, 0644, 0)) == SEM_FAILED) {
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


    while(1) {
	sem_wait(waitClientSem);

	printf("userNum : %d\n",ptr[0]);
	printf("ranNum : %d\n",ranNum);
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
	sem_post(waitServerPrintSem);

    }

    getchar();



    sem_close(startSem);
    exit(0);
}
