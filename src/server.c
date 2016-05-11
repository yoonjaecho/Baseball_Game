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
	int i;

	int* ptr;
	int* ptr2;

	sem_t* startSem;
	sem_t* clientNumSem;
	sem_t* test;

	int semVal;

	int zero = 0;

	int ranNum;

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
	
	sem_unlink("test");
	if((test = sem_open("test", O_CREAT, 0644, 0)) == SEM_FAILED) {
	    perror("Sem Failed");
	    exit(1);
	}



	sem_getvalue(startSem, &semVal);
	printf("startSem : %d\n",semVal);

	ranNum = randomGenerator();

	printf("Random number : %d\n",ranNum);
	
	sem_wait(startSem);
	printf("Client hi\n");
	
	sem_getvalue(startSem, &semVal);
	printf("startSem : %d\n",semVal);
	sem_wait(startSem);
	
	printf("Client hi\n");
	sem_getvalue(startSem, &semVal);
	printf("startSem : %d\n",semVal);

	printf("=========================\n");
	printf("        Game Start       \n");
	printf("=========================\n");

	getchar();
	
	if((startSem = sem_open("startSem", O_CREAT, 0644, -100)) == SEM_FAILED) {
	    perror("Sem Failed");
	    exit(1);
	}


	sem_close(startSem);
	exit(0);
}
