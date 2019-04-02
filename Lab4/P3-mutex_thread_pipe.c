/*
   ----- 48450 -- week 4 lab handout 3 ------ 
By programing the handout 1, you can try to add the semaphore to control the critical section of two threads' operation: if the runnerOne is running, the runnerTwo should wait. 
*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>

int sum;/*this data is shared by the threads*/
/* The mutex lock */
pthread_mutex_t mutex;
/* the semaphores */
sem_t one, two;
pthread_t tid1, tid2; //Thread ID
pthread_attr_t attr; //Set of thread attributes

#define MESSLENGTH 80

char pourIntoPipe[] = "This has been through the pipe!\n";

char collectFromPipe[MESSLENGTH];
void *runnerOne(void *param);/*threads call this function*/
void *runnerTwo(void *param);/*threads call this function*/
void initializeData();

int main(int argc, char*argv[])
{
	// used to store pipe pointer
	int  fd[2];

	initializeData();

	if (pipe(fd) < 0)
		perror("pipe error");

	/*get the default attributes*/
	pthread_attr_init(&attr);
	/*create the thread 1*/
	pthread_create(&tid1, &attr, runnerOne, fd);

	sleep (1);

	/*create the thread 2*/
	pthread_create(&tid2, &attr, runnerTwo, fd);

	/*wait for the thread to exit*/
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	close(fd[0]);
	close(fd[1]);
}

/*The thread will begin control in this function*/
void *runnerOne(void *param)
{
	int *fd = (int*)param;
	/* aquire the mutex lock */
	pthread_mutex_lock(&mutex);

	// write content to pipe
	write(fd[1], pourIntoPipe, strlen(pourIntoPipe));

	printf("thread one: write to pipe\n");
	/* release the mutex lock */
	pthread_mutex_unlock(&mutex);

}

/*The thread will begin control in this function*/
void *runnerTwo(void *param)
{
	int *fd = (int*)param;
	/* aquire the mutex lock */
	pthread_mutex_lock(&mutex);

	// read content from the pipe
	read(fd[0], collectFromPipe, MESSLENGTH);
	printf("%s", collectFromPipe);

	printf("thread two: read from pipe\n");
	/* release the mutex lock */
	pthread_mutex_unlock(&mutex);
}

void initializeData() {
	sum = 0;

	/* Create the mutex lock */
	pthread_mutex_init(&mutex, NULL);

	/* Get the default attributes */
	pthread_attr_init(&attr);
}
