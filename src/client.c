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
	int semVal;

	int userNum;

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

	sem_getvalue(startSem, &semVal);
	printf("startSem : %d\n",semVal);

	printf("====================\n");
	printf("     Hello \n");
	printf("     Game Start!!\n");
	printf("====================\n");

	sem_post(startSem);

	
	close(fd);
	close(fd2);
}
