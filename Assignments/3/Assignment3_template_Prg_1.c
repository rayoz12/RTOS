/* 
* RTOS Autumn 2019
* Assignment 3 Program_1 template
*
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
//Averages calculated
double avg_wait_t = 0.0, avg_turnaround_t = 0.0;
//Semaphore
sem_t sem_RR;

/* --- Prototypes --- */

void remove_element(Process** array, int index, int array_length)
{
   int i;
   for(i = index; i < array_length - 1; i++) 
   {
	   array[i] = array[i + 1];
   }
}

/* Initializes data and utilities used in thread params */
void initializeData(ThreadParams* params)
{
	int err;
	params->quantum_t = 4;

	if((err = (sem_init(&sem_RR, 0, 0)) !=0))
	{	
	    perror("semaphore initialize erro \n");
	    exit(err);
	}
	
	//init fifo pipe


	//init processes
	Processes[0].pid = 1; Processes[0].arrive_t = 8; Processes[0].burst_t = 10;
	Processes[1].pid = 2; Processes[1].arrive_t = 10; Processes[1].burst_t = 3;
	Processes[2].pid = 3; Processes[2].arrive_t = 14; Processes[2].burst_t = 7;
	Processes[3].pid = 4; Processes[3].arrive_t = 9; Processes[3].burst_t = 5;
	Processes[4].pid = 5; Processes[4].arrive_t = 16; Processes[4].burst_t = 4;
	Processes[5].pid = 6; Processes[5].arrive_t = 21; Processes[5].burst_t = 6;
	Processes[6].pid = 7; Processes[6].arrive_t = 26; Processes[6].burst_t = 2;
	
	//Initialise remaining time to be same as burst time
	for (int i = 0; i < PROCESSNUM; i++) {
		Processes[i].remain_t = Processes[i].burst_t;
	}
}


/* this function calculates CPU RR scheduling, writes waiting time and turn-around time to th FIFO */
void* worker1(void *params)
{
   // add your code here
	ThreadParams* threadParams = (ThreadParams *) params;

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
		for (size_t i = 0; i < PROCESSNUM; i++)
		{
			Process* process = &Processes[i];
			if (process->arrive_t == CPUTime)
			{
				printf("Process arrived: %d\n", process->pid);
				activeProcessList[activeProcessLen] = process;
				activeProcessLen++;
			}
		}

		//check if we have finished
		int finished = 1;
		for (size_t i = 0; i < PROCESSNUM; i++)
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
					activeProcess->completion_t = CPUTime - 1;
					//printf("Process Completed: %d, Completetion: %d\n", activeProcess->pid, activeProcess->completion_t);
					
					//remove from active process, no need to increment.
					remove_element(activeProcessList, activeProcessIdx, activeProcessLen);
					activeProcessLen--;
					//since we removed we have to check if he array is empty
					if (activeProcessLen == 0)
					{
						printf("Setting to NULL");
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
			printf("CPU Time: %d, CPU IDLE\n", CPUTime);
		}
		else
		{
			printf("CPUTime: %d, Executing Process ID: %d, Remain Time: %d\n", CPUTime, activeProcess->pid, activeProcess->remain_t);
			//printf("");
			activeProcess->remain_t--;
			quantumTimer--;
		}
		

		
	}

	//calculate wait and turn around time for each process
	for (size_t i = 0; i < PROCESSNUM; i++)
	{
		Process process = Processes[i];
		process.turnaround_t = process.completion_t - process.arrive_t;
		process.wait_t = process.turnaround_t - process.burst_t;
		printf("Process ID: %d, Turnaround: %d, Wait: %d\n", process.pid, process.turnaround_t, process.wait_t);
	}

	printf("Finished Worker1!\n");
	

	return NULL;
}

/* reads the waiting time and turn-around time through the FIFO and writes to text file */
void* worker2()
{
   // add your code here

   return NULL;
}

/* this main function creates named pipe and threads */
int main(void)
{
	int err;
	pthread_t tid[2];
	pthread_attr_t attr[2];
	ThreadParams params;
	/* creating a named pipe(FIFO) with read/write permission */
	// add your code 

	/* initialize the parameters */
	 // add your code 
	
	initializeData(&params);

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
	
	if((err = (pthread_join(tid[0], NULL)!=0)))
	{
	    perror("join thread 1 error\n");
	    return -3;
	}
	if((err = (pthread_join(tid[1], NULL)!=0)))
	{
	    printf("join thread 2 error\n");
	    return -4;
	}

	/* wait for the thread to exit */
	//add your code
	
	return 0;
}
