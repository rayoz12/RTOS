/*************************************************************
----- 48450 -- week 6 lab handout 1 ------ 
By programing the handout 1, you will follow the lecture notes to implement the CPU scheduling policy changing. 
*************************************************************/

#include <sys/time.h>
#include <sys/resource.h>
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


#define ITERATIONS 10000000

void printSchedulingPolicy(void);


int main(void)
{     

  struct sched_param p;

	struct rusage usage;
	long long i;
	int A[1000000];
	
	printf("Before change policy: ");
	printSchedulingPolicy();
	/* change the CPU scheduling policy*/
	
/* experiment with value of priority below: try setting as high as 98 and then 99! */
	p.sched_priority = 99;   

/************************************** 
You can change replace this code line with 'if (sched_setscheduler(0, SCHED_RR, &p) != 0)' {
****************************************/

	if (sched_setscheduler(0, SCHED_FIFO, &p) != 0) {
		printf("Failed to set the scheduler\n");
	}	   	
	printf("After change policy: ");
	printSchedulingPolicy();

	exit(0);
}/* end of Main */


/********************Function Call *******************/

/*print out the current CPU scheduling policy in operation system*/

void printSchedulingPolicy(void)
{
  	int which;
	/* get the current CPU scheduling policy */
	which = sched_getscheduler(0);
	switch (which) {
	case SCHED_OTHER: 
			printf("default scheduling is being used\n");		
		  	break;
	case SCHED_FIFO:
			printf("fifo scheduling is being used\n");		
		  	break;
	case SCHED_RR:		
			printf("round robin scheduling is being used\n");		
		  	break;
	}		
}
