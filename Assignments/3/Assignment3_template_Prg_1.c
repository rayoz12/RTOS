/* 
* RTOS Autumn 2019
* Assignment 3 Program_1 template
* Ryan Pereira - 090186
*/

/**
 * @file Assignment3_template_Prg_1.c
 * @author Ryan Pereira
 * @brief 
 * @version 0.1
 * @date 2019-05-22
 * 
 * This is an implementation of the round robin scheduling algorithm using a *request queue*. 
 * This was done on the assumption that a FIFO queue would be the mechanism to order the processes.
 * There is an alternate implementation in Assigment3_template_Prg_1.circular.c using a circular buffer.
 */

/*
	Building and Running the program is facilitated by a makefile which outputs to Assignment3_template_Pg_1.out:
	make runPg1
	OR
	make buildPg1,
	./Assignment3_template_Pg_1.out
	OR
	gcc -g -Wall Assignment3_template_Prg_1.c -o Assignment3_template_Prg_1.out -pthread
	./Assignment3_template_Pg_1.out
*/
/*
Usage:
	Pass in the time quantum and output file arguments as shown below.
	./Assignment3_template_Prg1.out quantum outputFile
	Example:
	./Assignment3_template_Prg1.out 4 output.txt
*/

#include <pthread.h> 	/* pthread functions and data structures for pipe */
#include <unistd.h> 	/* for POSIX API */
#include <stdlib.h> 	/* for exit() function */
#include <stdio.h>		/* standard I/O routines */
#include <stdbool.h>
#include <string.h>
#include <semaphore.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>

/*
Arrival Time:       Time at which the process arrives in the ready queue.
Completion Time:    Time at which process completes its execution.
Burst Time:         Time required by a process for CPU execution.
Turn Around Time:   Time Difference between completion time and arrival time.          
     Turn Around Time = Completion Time - Arrival Time

Waiting Time(W.T): Time Difference between turn around time and burst time.
     Waiting Time = Turn Around Time - Burst Time
*/

typedef struct RR_Params {
	//add your variables here
	int quantum_t;
	//FIFO
	char* FIFOFile;// = "/tmp/Assignment3RRFIFO";

	//Output File
	char* OutputFile;

} ThreadParams;

typedef struct Process
{
	//process id
	int pid;
	//process time
	int arrive_t, wait_t, burst_t, turnaround_t, remain_t, completion_t;


} Process;

//Max number of processes
#define PROCESSNUM 7
//Array of processes with 1 extra for placeholder remain_t
Process Processes[PROCESSNUM + 1];
//Semaphore
sem_t sem_RR;

/* --- Prototypes --- */

/**
 * remove_element
 * @brief Removes a specifed element by index from an array, shifiting elements up to fill the hole.
 * 
 * @param array The array to operator on
 * @param index The index to remove
 * @param array_length the length of the array;
 */
void remove_element(Process** array, int index, int array_length)
{
   int i;
   for(i = index; i < array_length - 1; i++) 
   {
	   array[i] = array[i + 1];
   }
}

/**
 * insert_process
 * @brief Inserts a process at the end of an array of processes, updating the active index and length to emulate a FIFO Queue.
 * 
 * @param array The array of processes to operate on.
 * @param process A pointer to the process to insert
 * @param activeProcessIdx The current active Index 
 * @param activeProcessLen The current length of the address.
 */
void insert_process(Process** array, Process* process, int* activeProcessIdx, int* activeProcessLen)
{
	//shift down
	int i;
	for (i = *activeProcessLen; i > *activeProcessIdx; i--)
	{
		array[i] = array[i - 1];
	}
	array[*activeProcessIdx] = process;
	(*activeProcessIdx)++;
	(*activeProcessLen)++;
}

/* Initializes data and utilities used in thread params */
/**
 * initializeData
 * @brief Initializes data and utilities used in thread params
 * 
 * @param params A pointer to ThreadParms to initialise
 * @param quantum The time quantum to set.
 * @param outputFile The Output file.
 */
void initializeData(ThreadParams* params, int quantum, char* outputFile)
{
	int err;

	//init params
	params->quantum_t = quantum;
	params->FIFOFile = "/tmp/Assignment3RRFIFO";
	params->OutputFile = outputFile;


	if((err = (sem_init(&sem_RR, 0, 0)) !=0))
	{	
	    perror("semaphore initialize erro \n");
	    exit(err);
	}


	//init processes
	Processes[0].pid = 1; Processes[0].arrive_t = 8; Processes[0].burst_t = 10;
	Processes[1].pid = 2; Processes[1].arrive_t = 10; Processes[1].burst_t = 3;
	Processes[2].pid = 3; Processes[2].arrive_t = 14; Processes[2].burst_t = 7;
	Processes[3].pid = 4; Processes[3].arrive_t = 9; Processes[3].burst_t = 5;
	Processes[4].pid = 5; Processes[4].arrive_t = 16; Processes[4].burst_t = 4;
	Processes[5].pid = 6; Processes[5].arrive_t = 21; Processes[5].burst_t = 6;
	Processes[6].pid = 7; Processes[6].arrive_t = 26; Processes[6].burst_t = 2;
	
	//Initialise remaining time to be same as burst time
	int i;
	for (i = 0; i < PROCESSNUM; i++) 
	{
		Processes[i].remain_t = Processes[i].burst_t;
	}
}

/**
 * sendRRDataFIFO
 * @brief Send and write average wait time and turnaround time to fifo
 * 
 * @param avg_turnaround_t The turnaround time to send.
 * @param avg_wait_t The wait time to send
 * @param FIFOFile The name of the fifo file to write to.
 */
void sendRRDataFIFO(double avg_turnaround_t, double avg_wait_t, char* FIFOFile) {
	int res, fifofd;
	
	//char* RRFIFO = "/tmp/Assignment3RRFIFO";
	
	res = mkfifo(FIFOFile, 0777);
	
	if (res < 0) {
		perror("mkfifo error\n");
		exit(0);
	}
	
	sem_post(&sem_RR);
	
	fifofd = open(FIFOFile, O_WRONLY);
	
	if (fifofd < 0) {
		perror("fifo open send error\n");
		exit(0);
	}
	
	write(fifofd, &avg_wait_t, sizeof(avg_wait_t));
	write(fifofd, &avg_turnaround_t, sizeof(avg_turnaround_t));
	
	close(fifofd);
	
	unlink(FIFOFile);
}

/**
 * @brief this function calculates CPU RR scheduling, writes waiting time and turn-around time to the FIFO
 * 
 * @param params A pointer to the ThreadParams
 * @return void* Returns null as a the system expects.
 */
void* worker1(void *params)
{
	int i;
   	// add your code here
	ThreadParams* threadParams = (ThreadParams*) params;

	Process* activeProcessList[PROCESSNUM];
	int activeProcessIdx = 0;
	int activeProcessLen = 0;
	Process* activeProcess = NULL;
	//if we are currently in a process.
	int inActiveJob = 0;
	//the current MS of the CPU
	int CPUTime = 0;
	int quantumTimer = threadParams->quantum_t;
	//flag that is set when everything is done.
	int done = 0;

	while (!done) {
		CPUTime++;

		/**
		 * This loop checks goes in an order of 3 things
		 * Check if an processes have arrived and add it to the activeProcessList
		 * Check if the job on the previous tick has finished or the quantum has passed and update the active process  
		 * if the active process is defined execute it 
		 */

		//check if any processes have arrived
		
		for (i = 0; i < PROCESSNUM; i++)
		{
			Process* process = &Processes[i];
			if (process->arrive_t == CPUTime)
			{
				printf("CPUTime: %d, Process arrived: %d\n", CPUTime, process->pid);
				//inserts it as if we are tracking a circular buffer.
				// activeProcessList[activeProcessLen] = process;
				// activeProcessLen++;
				//inserts it as if we were tracking a FIFO Queue
				insert_process(activeProcessList, process, &activeProcessIdx, &activeProcessLen);
			}
		}

		//check if we have finished
		int finished = 1;
		for (i = 0; i < PROCESSNUM; i++)
		{
			if (Processes[i].remain_t != 0)
			{
				finished = 0;
				break;
			}
		}
		if (finished)
			done = 1;
		
		
		//check if we are in an active job
		if (inActiveJob)
		{
			//check if the job is finished or the time quantum has passed 
			if (activeProcess->remain_t == 0 || quantumTimer <= 0)
			{
				//check if the process has finished and remove it fom the active process list
				if (activeProcess->remain_t == 0)
				{
					//job has finished
					activeProcess->completion_t = CPUTime;
					printf("CPUTime: %d, Process Completed: %d\n", CPUTime, activeProcess->pid);
					
					//remove from active process, no need to increment.
					remove_element(activeProcessList, activeProcessIdx, activeProcessLen);
					activeProcessLen--;
					//since we removed we have to check if he array is empty
					if (activeProcessLen == 0)
					{
						activeProcess = NULL;
						inActiveJob = 0;
					}
					else
					{
						//no need to increment
						activeProcessIdx = activeProcessIdx >= activeProcessLen ? 0 : activeProcessIdx;
						activeProcess = activeProcessList[activeProcessIdx];
						quantumTimer = threadParams->quantum_t;
					}
					
				}
				else
				{
					//process hasn't finished but we need to move on
					activeProcessIdx = activeProcessIdx + 1 >= activeProcessLen ? 0 : activeProcessIdx + 1;
					activeProcess = activeProcessList[activeProcessIdx];
					quantumTimer = threadParams->quantum_t;
				}
			}
			else
			{
				//do nothing to just execute the current process?
			}
			
		}
		else
		{
			//schedule a job
			if (activeProcessLen > 0)
			{
				activeProcess = activeProcessList[0];
				activeProcessIdx = 0;
				inActiveJob = 1;
			}
		}
		

		//process the current job

		if (activeProcess == NULL)
		{
			printf("CPUTime: %d, CPU IDLE\n", CPUTime);
		}
		else
		{
			activeProcess->remain_t--;
			quantumTimer--;
			printf("CPUTime: %d, Executing Process ID: %d, Remain Time: %d, Quantum Time: %d\n", CPUTime, activeProcess->pid, activeProcess->remain_t, quantumTimer);
		}
	}

	//Averages calculated
	double avg_wait_t = 0.0f;
	double avg_turnaround_t = 0.0f;

	//calculate wait and turn around time for each process
	for (i = 0; i < PROCESSNUM; i++)
	{
		Process process = Processes[i];
		process.turnaround_t = process.completion_t - process.arrive_t;
		process.wait_t = process.turnaround_t - process.burst_t;
		printf("Process ID: %d, Turnaround: %d, Wait: %d\n", process.pid, process.turnaround_t, process.wait_t);
		avg_turnaround_t += process.turnaround_t;
		avg_wait_t += process.wait_t;
	}

	avg_turnaround_t /= PROCESSNUM;
	avg_wait_t /= PROCESSNUM;

	printf("Write to FIFO: Average wait time: %fs\n", avg_wait_t);
	
	printf("Write to FIFO: Average turnaround time: %fs\n", avg_turnaround_t);

	sendRRDataFIFO(avg_turnaround_t, avg_wait_t, threadParams->FIFOFile);

	printf("Finished Worker 1\n");
	

	return NULL;
}

/**
 * @brief reads the waiting time and turn-around time through the FIFO and writes to text file 
 * 
 * @param params The thread params
 * @return void* 
 */
void* worker2(void* params)
{
	int err;
	ThreadParams* threadParams = (ThreadParams* ) params;
   	// add your code here

	sem_wait(&sem_RR);
	printf("Worker thread 2 Started\n");

	int fifofd;
	
	double fifo_avg_turnaround_t,
		fifo_avg_wait_t;
	
	//char * RRFIFO = "/tmp/Assignment3RRFIFO";	
	fifofd = open(threadParams->FIFOFile, O_RDONLY);
	
	if (fifofd < 0) {
		perror("fifo open read error\n");
		exit(0);
	}
	
	if ((err = (read(fifofd, &fifo_avg_wait_t, sizeof(double)))) < 0) {
		perror("Failed to read Average Wait time from FIFO:");
		exit(0);
	}
	if ((err = (read(fifofd, &fifo_avg_turnaround_t, sizeof(double)))) < 0) {
		perror("Failed to read average turnaround time from FIFO:");
		exit(0);
	}

	printf("Read from FIFO: Average wait time %fs\n", fifo_avg_wait_t);
	printf("Read from FIFO: Average turnaround time %fs\n", fifo_avg_turnaround_t);

	//close fifo	
	close(fifofd);
	//remove the file
	remove(threadParams->FIFOFile);

	//open the output file
	FILE* fp = fopen(threadParams->OutputFile, "w");
	if (fp == NULL) {
		perror("Failed to open FIFO output file:");
		exit(-1);
	}

	fprintf(fp, "Average wait time: %fs \n", fifo_avg_wait_t);
	fprintf(fp, "Average turnaround time: %fs \n", fifo_avg_turnaround_t);

	fclose(fp);

	printf("Finished Worker 2\n");

   	return NULL;
}

/* this main function creates named pipe and threads */
int main(int argc, char* argv[])
{
	int err;
	//The Threads
	pthread_t tid[2];
	//The attributes for the threads
	pthread_attr_t attr[2];
	ThreadParams params;

	if (argc != 3)
	{
		printf("\
Usage: \n\
	Pass in the time quantum and output file arguments as shown below. \n\
	./Assignment3_template_Prg1.out quantum outputFile \n\
	Example:\n\
	./Assignment3_template_Prg1.out 4 output.txt \n");
		exit(0);
	}

	/* creating a named pipe(FIFO) with read/write permission */
	/* initialize the parameters */
	// Get the input data
	int quantum;
	sscanf(argv[1], "%d", &quantum);
	initializeData(&params, quantum, argv[2]);
	
	printf("Quantum Time: %d\n", quantum);
	printf("Output File: %s\n", argv[2]);

	//initializeData(&params, 4, "output.txt");

	//Initialise the thread attributes
	pthread_attr_init(&attr[0]);
	pthread_attr_init(&attr[1]);

	/* create threads */
	 // add your code
	if((err = pthread_create(&(tid[0]), &attr[0], &worker1, (void*)(&params))) != 0)
 	{
	    perror("Worker 1 created error\n");
	    return err;
	}
	if((err = pthread_create(&(tid[1]), &attr[1], &worker2, (void*)(&params))) != 0)
	{
	    perror("Worker 2 created error\n");
	    return err;
	}
	
	/* wait for the thread to exit */
	//add your code
	if((err = (pthread_join(tid[0], NULL)!=0)))
	{
	    perror("join thread 1 error\n");
	    return err;
	}
	if((err = (pthread_join(tid[1], NULL)!=0)))
	{
	    printf("join thread 2 error\n");
	    return err;
	}

	
	return 0;
}
