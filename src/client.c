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

int main(int argc, char** argv)
{
    int fd,fd2;
    int i;

    int* ptr;
    int* ptr2;
    int flag = PROT_WRITE | PROT_READ;

    sem_t* startSem = sem_open("startSem", 0);
    sem_t* twoClientSem = sem_open("twoClientSem", 0);
    sem_t* aSem = sem_open("aSem", 0);
    sem_t* bSem = sem_open("bSem", 0);
    sem_t* clientInputSem = sem_open("clientInputSem", 0);
    sem_t* waitServerPrintSem = sem_open("waitServerPrintSem", 0);
    sem_t* allReadyToStartSem = sem_open("allReadyToStartSem", 0);

    int semVal;

    int userNum;

    int winCheck;


    if((fd = open("input.txt", O_RDWR, 0666)) < 0) {
	perror("File open error");
	exit(1);
    }

    if((ptr = (int *)mmap(0, 4096, flag, MAP_SHARED, fd ,0)) == NULL) {
	perror("mmap error");
	exit(1);
    }

    if((fd2 = open("output.txt", O_RDWR, 0666)) < 0) {
	perror("File open error");
	exit(1);
    }

    if((ptr2 = (int *)mmap(0, 4096, flag, MAP_SHARED, fd2 ,0)) == NULL) {
	perror("mmap error");
	exit(1);
    }

    /*
       sem_getvalue(aSem, &semVal);
       printf("aSem : %d\n",semVal);
     */


    sem_getvalue(twoClientSem, &semVal);

    if(!semVal) {
	printf("You can't enter this room! \n");
	exit(1);
    }

    sem_wait(twoClientSem);

    /* Starting game */

    printf("======================\n");
    printf("     Hello %d\n",getpid());
    printf("     Game Start!!\n");
    printf("======================\n");

    sem_post(startSem);
    sem_wait(allReadyToStartSem);

    winCheck = 0;

    while(1) {

	sem_getvalue(aSem, &semVal);
	if(!semVal) {
	    sem_wait(bSem);

	    /* print */
	    if(ptr2[0] == 3) {
		printf("[%d] %d : %d strike, %d ball\n",ptr2[2], ptr2[3], ptr2[0], ptr2[1]);
		if(ptr2[3] == getpid()) {
		    printf("You Win !!\n");
		} else {
		    printf("You Lose !!\n");
		}
		ptr2[4] = 1;
	    } else {
		printf("[%d] %d : %d strike, %d ball\n",ptr2[2], ptr2[3], ptr2[0], ptr2[1]);
	    }

	    sem_post(aSem);
	    sem_post(aSem);

	} else {

	    sem_wait(aSem);

	    /* input */
	    printf("Input Number(100 - 999) : ");
	    scanf("%d",&userNum);
	    ptr[0] = userNum;
	    ptr[1] = getpid();


	    sem_post(clientInputSem);
	    sem_wait(waitServerPrintSem);

	    sem_post(bSem);

	    /* print */
	    if(ptr2[0] == 3) {
		printf("[%d] %d : %d strike, %d ball\n",ptr2[2], ptr2[3], ptr2[0], ptr2[1]);
	    } else {
		printf("[%d] %d : %d strike, %d ball\n",ptr2[2], ptr2[3], ptr2[0], ptr2[1]);
	    }

	    sem_wait(aSem);

	}

    }


    close(fd);
    close(fd2);

    sem_post(twoClientSem);
}
