/***********************************************************************************/
//***********************************************************************************
//            *************NOTE**************
// This is a template for the subject of RTOS in University of Technology Sydney(UTS)
// Please complete the code based on the assignment requirement.

//***********************************************************************************
/***********************************************************************************/

/*
	Building and Running the program is facilitated by a makefile which outputs to assignment2.out:
	make run
	OR
	make build,
	./assignment2.out
*/
/*
	Usage:
	In your current working directory provide a data.txt input file that contains
	the input for the program.
	Then run the program with ./assignment2.out
	
	You can print out this usage information by running ./assignment2.out --help
*/
#include  <pthread.h>
#include  <stdlib.h>
#include  <unistd.h>
#include  <stdio.h>
#include  <sys/types.h>
#include  <fcntl.h>
#include  <string.h>
#include  <sys/stat.h>
#include  <semaphore.h>
#include  <sys/time.h>

#define MESSAGE_BUFFER_LENGTH 255

/* --- Structs --- */

typedef struct ThreadParams {
	int pipeFile[2];
	sem_t sem_read, sem_justify, sem_write;
	char message[MESSAGE_BUFFER_LENGTH];
	/*
	 * apparently this is not needed, it's given but we don't need to use it. 
	 * The semaphores already enforce mutual exclusion for execution of threads and thus resources
	 */
	pthread_mutex_t lock;
} ThreadParams;

/* --- Prototypes --- */

/* Initializes data and utilities used in thread params */
void initializeData(ThreadParams *params);

/* This thread reads data from data.txt and writes each line to a pipe */
void* ThreadA(void *params);

/* This thread reads data from pipe used in ThreadA and writes it to a shared variable */
void* ThreadB(void *params);

/* This thread reads from shared variable and outputs non-header text to src.txt */
void* ThreadC(void *params);

/* --- Shared Variables --- */
u_int8_t isfileReadFinished = 0;

/* --- Main Code --- */
int main(int argc, char const *argv[]) {
	//struct timeval t1, t2;
	//gettimeofday(&t1, NULL);  // Start Timer
	int err;
	pthread_t tid[3];
	pthread_attr_t attr[3];
	ThreadParams params;

	// Initialization
	initializeData(&params);
	pthread_attr_init(&attr[0]);
	pthread_attr_init(&attr[1]);
	pthread_attr_init(&attr[2]);

	// Create Threads
	if((err = pthread_create(&(tid[0]), &attr[0], &ThreadA, (void*)(&params))) != 0)
	{
		perror("Error creating threads: ");
		exit(-1);
	}
	if((err = pthread_create(&(tid[1]), &attr[1], &ThreadB, (void*)(&params))) != 0)
	{
		perror("Error creating threads: ");
		exit(-1);
	}
	if((err = pthread_create(&(tid[2]), &attr[2], &ThreadC, (void*)(&params))) != 0)
	{
		perror("Error creating threads: ");
		exit(-1);
	}
	//TODO: add your code
	if (argc >= 2)
	{
		if (strcmp(argv[1],"--help") == 0)
		{
			printf("\
Usage: \n\
	In your current working directory provide a data.txt input file that contains input for the program. \n\
	Then run the program with ./assignment2.out\n");
			exit(0);
		};
	}

	// Wait on threads to finish
	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);
	pthread_join(tid[2], NULL);
	//TODO: add your code

	printf("[Exit] Main Thread Finished\n");

	//cleanup
	close(params.pipeFile[0]);
	close(params.pipeFile[1]);
	
	sem_destroy(&params.sem_read);
	sem_destroy(&params.sem_justify);
	sem_destroy(&params.sem_write);

	return 0;
}

void initializeData(ThreadParams *params) {
	int err;
	// Initialize Sempahores
	/* Setting the inital value to one so that thread A runs straight away and
	 * Doesn't get stuck in a deadlock waiting inifitely long.
	 */
	if ((err = sem_init(&(params->sem_read), 0, 1)) != 0)
	{
		perror("Error Initialising Sem_Read: ");
		exit(-1);
	}
	if ((err = sem_init(&(params->sem_justify), 0, 0)) != 0)
	{
		perror("Error Initialising Sem_Justify: ");
		exit(-1);
	}
	if ((err = sem_init(&(params->sem_write), 0, 0)) != 0)
	{
		perror("Error Initialising Sem_Write: ");
		exit(-1);
	}
	//TODO: add your code

	/* Create the mutex lock */
	if ((err = pthread_mutex_init(&(params->lock), NULL)) != 0) 
	{
		perror("Error Initialising Mutex: ");
		exit(-1);
	}
	
	//create the pipe
	if (pipe(params->pipeFile) < 0)
	{
		perror("Error Initialising Pipe: ");
		exit(-1);
	};

	return;
}

void* ThreadA(void *params) {
	//TODO: add your code
	//Cast params pointer into struct
	ThreadParams* threadParams = (ThreadParams *) params;
	//the int result of the get line function
	ssize_t read;
	//the length of the line read
	size_t len;
	//a temporary variable to hold the line read
	char* line = NULL;
	int result;

	/* Init I/O for this thread */
	//open file
	FILE* fp = fopen("data.txt", "r");
	if (fp == NULL) {
		perror("Failed to open data.txt file");
		exit(-1);
	}
	printf("Successfully opened file \n");


	for (;;) 
	{
		sem_wait(&threadParams->sem_read); //wait here until other threads signal that they are done
		printf("\n------Running thread A------\n");

		read = getline(&line, &len, fp);

		//check if we have finished reading the file
		if (read == -1)
		{
			//Close the write end of the file to signify that we have finished reading from the file and are finished with the pipe.
			close(threadParams->pipeFile[1]); 
			sem_post(&threadParams->sem_justify); //signal the next thread
			break;
		}
		
		if ((result = write(threadParams->pipeFile[1], line, len) < 0)) {
			perror("Failed to write to pipe!");
			exit(EXIT_FAILURE);
		}
		printf("Pipe Write: %s", line);
		sem_post(&threadParams->sem_justify);
	}
	//thread cleanup
	fclose(fp);

	printf("[Exit] Thread A has Finished\n");
	return NULL;
}

void* ThreadB(void *params) {
	//TODO: add your code
	ThreadParams* threadParams = (ThreadParams *) params;

	int result;

	//exit infinite loop when the Pipe has closed and file read has finished
	for (;!isfileReadFinished;) {
		sem_wait(&threadParams->sem_justify);
		printf("\n------Running thread B------\n");
		//printf("Reading from pipe \n");
		if ((result = read(threadParams->pipeFile[0], threadParams->message, MESSAGE_BUFFER_LENGTH)) < 0) {
			perror("Failed to read Pipe");
			exit(EXIT_FAILURE);
		}
		printf("Pipe Read: %s", threadParams->message);



		if (result == 0)
		{
			printf("\nDetected Write pipe is closed! File read finished\n");
			isfileReadFinished = 1;
		}
		sem_post(&threadParams->sem_write);
	}

	printf("[Exit] Thread B has Finished\n");

	//thread cleanup
	return NULL;
}

void* ThreadC(void *params) {
	//TODO: add your code
	ThreadParams* threadParams = (ThreadParams *) params;
	int result;
	
	u_int8_t inContentRegion = 0;
	FILE* fp = fopen("src.txt","w");

	for (;isfileReadFinished != 1;) {
		sem_wait(&threadParams->sem_write);
		printf("\n------Running thread C------\n");

		if (inContentRegion) {
			//write line to file
			printf("Writing to src.txt file\n");
			if ((result = fprintf(fp, "%s", threadParams->message) < 0)) {
				perror("Failed to write to src.txt file");
				exit(EXIT_FAILURE);
			}
		}
		else {
			//check if the current line is the end of the header
			//check if we have reached the end of the header, by checking if end header is in the message.
			inContentRegion = strstr(threadParams->message, "end_header") != NULL;
			inContentRegion ? printf("Entering Content Region\n") : 0;
		}

		sem_post(&threadParams->sem_read);
	}

	//thread cleanup
	fclose(fp);
	printf("[Exit] Thread C has Finished\n");
	return NULL;
}
