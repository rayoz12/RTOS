/*
	----- 48450 -- week 5 lab handout 1 ------ 
By programing the handout 1, you will follow the lecture notes to implement the SJF. Through this Lab sample. Please answer the following question that is embeded in the code lines. 

This is a program to do the shortest job frsit (SJF) CPU scheduling 

					
               Process			Burst Time
			P0					8
			P1 					4
		    	P2					9
		    	P3					5
	    		P4					15
	    		P5					7
*/

#include<stdio.h>
#include<stdbool.h>
#include <stdlib.h>

/* a struct storing the information of each process */

typedef struct
{
     int pid;//process id
     float arrive_t, wait_t, burst_t, turnaround_t, start_t;//process time
}process;

//sort. prepare to do Short job first schedule

void sort(process p[], int start, int num);

void main()
{
     int i,k, j;//index
     int processNum=0;//process number
     float avg_wait_t = 0.0, avg_turnaround_t = 0.0, start_t = 0.0;//average waiting time, turnaround time and global start time
     process *pro;//store process information using our struct(include id, four types of time)

 /*------initialize the data: process id and time -------*/     
 
     printf("This is a program to run example data to show shortest job first scheduling.\n\n");
     processNum=6;
     pro= malloc(sizeof(process)*processNum);//allocate memory for valaible p
     pro[0].pid=1;pro[0].arrive_t=0;pro[0].burst_t=8;//ppt example data
     pro[1].pid=2;pro[1].arrive_t=0;pro[1].burst_t=4;//ppt example data
     pro[2].pid=3;pro[2].arrive_t=0;pro[2].burst_t=9;//ppt example data
     pro[3].pid=4;pro[3].arrive_t=0;pro[3].burst_t=5;//ppt example data
     pro[4].pid=5;pro[4].arrive_t=0;pro[4].burst_t=15;//adittional item
     pro[5].pid=6;pro[5].arrive_t=0;pro[5].burst_t=7;//adittional item

/*----------- sort the existing processes ------------*/	  
     //sort the existing processes
     sort (pro,0,processNum);
     
/*--- implementing the SJF algorithm on burst time ---*/
     for (i = 0; i<processNum; i++)
     {
	  pro[i].start_t=start_t;
	  
          start_t += pro[i].burst_t;//set the global start time to the end of the process done time.
          pro[i].wait_t = pro[i].start_t - pro[i].arrive_t;//set the wait time as CPU start time minus process arrive time

          pro[i].turnaround_t = pro[i].burst_t + pro[i].wait_t;//set turn around time as bust time plus wait time
          avg_wait_t += pro[i].wait_t;
          avg_turnaround_t += pro[i].turnaround_t;
     }

 /* --------------Display the results------------------ */
     avg_wait_t /= processNum;
     avg_turnaround_t /= processNum;
     printf("Process Schedule Table: \n");
     printf("\tProcess ID\tArrival Time\tBurst Time\tWait Time\tTurnaround Time\n");
     for (i = 0; i<processNum; i++)
          printf("\t%d\t\t%f\t%f\t%f\t%f\n", pro[i].pid,pro[i].arrive_t, pro[i].burst_t, pro[i].wait_t, pro[i].turnaround_t);
     printf("\nAverage wait time: %f", avg_wait_t);
     printf("\nAverage turnaround time: %f\n", avg_turnaround_t);
}/* main */

/******************* Call function ******************/
/* Question-1: what does this function achieve? Give your analysis by following the code lines and your comments. 
 * This function sorts the process list from the smallest burst time to the largest burst time.
 * It does this by iterating through the processes from start to finish incrementally.
 * In each of the top level iterations it iterates again through the following processes.
 * in this second iteration it does a 1 pass bubble sort of the processes moving the smallest burst time to the front.
 * by comparing the burst times and swapping the current and next process.
 */

void sort(process p[], int start, int num)
{
   int i,j;
   process temp;
   for (i = start; i<num; i++)
   {
        for (j = i+1; j<num; j++)
        {
           //swap the less burst time to the front
           if(p[i].burst_t > p[j].burst_t)//if the front is larger than the back, swap	
           {
                temp = p[i];
                p[i] = p[j];
                p[j] = temp;
           }
        }
   }
}//sort end

