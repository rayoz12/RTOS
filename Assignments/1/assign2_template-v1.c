/***********************************************************************************/
//***********************************************************************************
//            *************NOTE**************
// This is a template for the subject of RTOS in University of Technology Sydney(UTS)
// Please complete the code based on the assignment requirement.

//***********************************************************************************
/***********************************************************************************/

/*
  To compile prog_1 ensure that gcc is installed and run the following command:
  gcc prog_1.c -o prog_1 -lpthread -lrt -Wall

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

/* --- Structs --- */

typedef struct ThreadParams {
  int pipeFile[2];
  sem_t sem_read, sem_justify, sem_write;
  char message[255];
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

/* --- Main Code --- */
int main(int argc, char const *argv[]) {
  struct timeval t1, t2;
  gettimeofday(&t1, NULL);  // Start Timer
  int err;
  pthread_t tid[3];
  pthread_attr_t attr[3];
  ThreadParams params;

  // Initialization
  initializeData(&params);
  pthread_attr_init(&attr);

  // Create Threads
  if(err = pthread_create(&(tid[0]), &attr[0], &ThreadA, (void*)(&params)))
  {
  	perror("Error creating threads: ");
  	fprintf(stderr, "Error No. %d", err);
	exit(-1);
  }
  if(err = pthread_create(&(tid[1]), &attr[1], &ThreadB, (void*)(&params)))
  {
  	perror("Error creating threads: ");
  	fprintf(stderr, "Error No. %d", err);
	exit(-1);
  }
  if(err = pthread_create(&(tid[2]), &attr[2], &ThreadC, (void*)(&params)))
  {
  	perror("Error creating threads: ");
  	fprintf(stderr, "Error No. %d", err);
	exit(-1);
  }
  //TODO: add your code
 

  // Wait on threads to finish
  pthread_join(tid[0], NULL);
  pthread_join(tid[1], NULL);
  pthread_join(tid[2], NULL);
  //TODO: add your code

  return 0;
}

void initializeData(ThreadParams *params) {
  int err;
  // Initialize Sempahores
  /* Setting the inital value to one so that thread A runs straight away and
   * Doesn't get stuck in a deadlock waiting inifitely long.
   */
  if (err = sem_init(&(params->sem_read), 0, 1)) 
  {
  	perror("Error Initialising Sem_Read: ");
  	fprintf(stderr, "Error No. %d", err);
	exit(-1);
  }
  if (err = sem_init(&(params->sem_justify), 0, 0))
  {
  	perror("Error Initialising Sem_Justify: ");
  	fprintf(stderr, "Error No. %d", err);
	exit(-1);
  }
  if (err = sem_init(&(params->sem_write), 0, 0))
  {
  	perror("Error Initialising Sem_Write: ");
  	fprintf(stderr, "Error No. %d", err);
	exit(-1);
  }
  //TODO: add your code

  /* Create the mutex lock */
  if (err = pthread_mutex_init(&(params->lock), NULL))
  {
  	perror("Error Initialising Mutex: ");
  	fprintf(stderr, "Error No. %d", err);
	exit(-1);
  }
  
  //create the pipe
  pipe(params->pipeFile);
  return;
}

void* ThreadA(void *params) {
  //TODO: add your code
  ThreadParams* threadParams = (ThreadParams *) params;
  FILE* fp = fopen("data.txt", "r");
  if (fp == NULL) {
    perror("Failed to open data file");
    exit(-1);
  }

  int lineNumber = 0;

  for (;;) {
    sem_wait(&threadParams->sem_read);
    pthread_mutex_lock(&threadParams->lock);
    if ((read = getline(threadParams->message, &len) {

    }

    sem_post(&threadParams->sem_justify)
  }
}

void* ThreadB(void *params) {
  //TODO: add your code
  
}

void* ThreadC(void *params) {
  //TODO: add your code
}
