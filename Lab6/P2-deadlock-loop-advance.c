/*
----- 48450 -- week 6 lab handout 2 ------ 
By programing the handout 2, you will follow the lecture notes to implement the deadlock detection algorithm with the loop search method. 

This program code is for detecting the deadlock process
*/
 
#include<stdio.h>
int Request[5][3]; //the process request
int Allocation[5][3];// already allocate resources to process
int Available[3];//available instances for each resource
int p,r;//the no. of process and resource

void input_example();
void cal();

/* ************************* Main ************************* */
int main()
{
    printf("\n********** Deadlock Detection Algo ************\n");
    input_example();
    cal();//detect deadlock
    return 0;
} /* end of Main */

/******************* Call functions ******************/
//call function-1: input example
void input_example()
{
    int i,j;
    p=5;//Process No.
    r=3;//Resource no.
    //Enter the Request Matrix
    Request[0][0]=0;Request[0][1]=0;Request[0][2]=0;
    Request[1][0]=2;Request[1][1]=0;Request[1][2]=2;
    Request[2][0]=0;Request[2][1]=0;Request[2][2]=0;
    Request[3][0]=1;Request[3][1]=0;Request[3][2]=0;
    Request[4][0]=0;Request[4][1]=0;Request[4][2]=2;

    //Enter the Allocation Matrix
    Allocation[0][0]=0;Allocation[0][1]=1;Allocation[0][2]=0;
    Allocation[1][0]=2;Allocation[1][1]=0;Allocation[1][2]=0;
    Allocation[2][0]=3;Allocation[2][1]=0;Allocation[2][2]=3;
    Allocation[3][0]=2;Allocation[3][1]=1;Allocation[3][2]=1;
    Allocation[4][0]=0;Allocation[4][1]=0;Allocation[4][2]=2;

    //Enter the available Resources
    Available[0]=0;Available[1]=0;Available[2]=0;
}

//call function-2: calculate dead-lock
void cal()
{
    int finish[5],dead[5];//store whether the process finish and dead process id.
    int k,i,j,deadlockNum,flag;//k,i,j is index variable. flag use for whether there is deadlock
    int runOrder[5];
    for(i=0;i<p;i++)
    {
         finish[i]=0;
    }

/* -----------detect deadlock process------------ */
    flag=0;//whether there is a deadlock: 0 is no deadlock, 1 is deadlock
    int pro_id=0;
    while(!flag && pro_id<p)
    {
		int possbileLock=1;
		for(i=0;i<p;i++)// for every process, to check whethe it is OK to run
		{	
		   int c=0;
		   for(j=0;j<r;j++)//for every resoure
		   { 
	if(Request[i][j]<=Available[j] && finish[i]==0)
			  {
					c++;
					if(c==r)
					{
						flag=0;//not deadlock
/* if it can be done, the allocated resources can be released after finish */
						for(k=0;k<r;k++)
						{
						   Available[k]+=Allocation[i][k];//release the allocation resource of process i
						   finish[i]=1;//if it is done, set finish to 1
						}
						possbileLock=0;
						runOrder[pro_id]=i;//record the current finish process id
								 
					}
			   }
			   else// if there is one resource not satisfy the request, it is deadlock
			   {
					possbileLock=1;
					//printf("possible deadlock %d\n", i);
					break;
			   }
		    }//for j end
		    // if there is one process can run, go out and run the process
		    if(possbileLock==0)
			break;
 		}//for i end
		if(i==p && possbileLock==1)//if i loop for the whole processes and still in deadlock.
		{
			flag=1; break;
		}
		if(flag)
			break;//if there is deadlock
		pro_id++;
     }//while end
     

/* --------------display deadlock process----------------- */
     if(flag==1)
     {
         printf("\nSystem is in Deadlock!\n");
	 // output the deadlock processes
	 printf("****************************************\n");
	 printf("Deadlock processes are: ");
	 for(i=0;i<p;i++)
	 {
		if(finish[i]==0)
		    printf("P%d ",i);
	 }
	 printf("\n****************************************\n\n");
     }
     else
     {   
	 printf("\nHappy finish. No Deadlock Occur!\n");
	 printf("****************************************\n");
	 printf("The running sequence is:  ");
	 for(i=0;i<p;i++)
	 {
	    printf("P%d",runOrder[i]);
	    if(i<p-1)
		printf("->");
	 }
	 printf("\n****************************************\n\n");
         
     }  
}

