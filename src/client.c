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
    sem_t* clientNumSem = sem_open("clientNumSem", 0);
    sem_t* inputNumSem = sem_open("inputNumSem", 0);
    sem_t* waitClientSem = sem_open("waitClientSem", 0);
    sem_t* waitServerPrintSem = sem_open("waitServerPrintSem", 0);

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
    sem_getvalue(startSem, &semVal);
    printf("startSem : %d\n",semVal);


    sem_getvalue(clientNumSem, &semVal);
    printf("clientNumSem : %d\n",semVal);
    */

    if(!semVal) {
	printf("You can't enter this room! Two clients already using.\n");
	exit(1);
    }

    sem_wait(clientNumSem);
/*
    sem_getvalue(clientNumSem, &semVal);
    printf("clientNumSem : %d\n",semVal);
*/
    /* Starting game */

    printf("======================\n");
    printf("     Hello [%d]\n",getpid());
    printf("     Game Start!!\n");
    printf("======================\n");

    sem_post(startSem);

    winCheck = 0;

    while(1) {

	//sem_wait(inputNumSem);
	sem_post(waitClientSem);

	printf("Input Number(100 - 999) : ");
	scanf("%d",&userNum);
	ptr[0] = userNum;

	sem_wait(waitServerPrintSem);
	printf("stirke : %d, ball : %d\n",ptr2[0], ptr2[1]);

	if(ptr2[0] == 3) {
	    printf("[%d] %d : %d strike, %d ball\n",getpid(), userNum, ptr2[0], ptr2[1]);
	    printf("You Win\n");
	    winCheck++;
	} else {
	    printf("[%d] %d : %d strike, %d ball\n",getpid(), userNum, ptr2[0], ptr2[1]);
	}
	
	//sem_post(inputNumSem);
	if(winCheck) {

	}
    }

    getchar();


    close(fd);
    close(fd2);

    sem_post(clientNumSem);
}
