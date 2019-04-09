/*
----- 48450 -- week 5 lab handout 2 ------ 
By programing the handout 2, you will follow the lecture notes to implement the deadlock detection algorithm. Through this Lab sample. Please answer the following questions that are embeded in the code lines. 

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
    //Enter the Request Matrix, Proccess by Resource
    Request[0][0]=0;Request[0][1]=0;Request[0][2]=0; //first process
    Request[1][0]=2;Request[1][1]=0;Request[1][2]=2; //second process
    Request[2][0]=0;Request[2][1]=0;Request[2][2]=0;
    Request[3][0]=1;Request[3][1]=0;Request[3][2]=0;
    Request[4][0]=0;Request[4][1]=0;Request[4][2]=2;

    //Enter the Allocation Matrix
    Allocation[0][0]=0;Allocation[0][1]=1;Allocation[0][2]=0;
    Allocation[1][0]=2;Allocation[1][1]=0;Allocation[1][2]=0;
    Allocation[2][0]=3;Allocation[2][1]=0;Allocation[2][2]=3;
    Allocation[3][0]=2;Allocation[3][1]=1;Allocation[1][2]=1;
    Allocation[4][0]=0;Allocation[4][1]=0;Allocation[2][2]=2;

    //Enter the available Resources
    Available[0]=0;Available[1]=0;Available[2]=0;
}

//call function-2: calculate dead-lock
void cal()
{
    int finish[5],dead[5];//store whether the process finish and dead process id.
    int k,i,j,deadlockNum,flag;//k,i,j is index variable. flag use for whether there is deadlock

    for(i=0;i<p;i++)
    {
         finish[i]=0;
    }

/* -----------detect deadlock process------------ */
    flag=0;//whether there is a deadlock: 0 is no deadlock, 1 is deadlock
    i=0;
    int pro_id=0;
    while(!flag && pro_id<p) //while no deadlock and process id is less than number of processees
    {
	if(pro_id==1)i=2;
	if(pro_id==2)i=3;
	if(pro_id==3)i=1;
       int c=0;
       for(j=0;j<r;j++)//for every resource
       { 
/* Question-1: What does the following code line acheive? Why do we need 'Request[i][j]<=Available[j])' this code line?  */
/* This code checks if a certain number of resources (j) requested by the process (i) is less than or equal to the number of currently avilable resources.
 * Essentially it checks if there is a resource deadlock or not.
 * we need this code line because it checks if there are enough resources available if there are it continues on 
 * to incre
 */
          if(Request[i][j]<=Available[j])
          {
             c++; //C is reset at the while loop
/* Question-2 What does the following code line acheive? */
/* This if statement is executed once we have checked that the process has all the different types of resources available.
 * The C variable keeps a counter of how many resources we have checked so far and compared to the
 * number of different types of resources we have available (r). Once all resources have been confirmed 
 * the process can finish and it's resources can be freed.
 */
            if(c==r)
             {
				flag=0;//not deadlock
		        //if it can be done, the allocated resources can be released after finish
                for(k=0;k<r;k++)
                {
                   Available[k]+=Allocation[i][k];//release the allocation resource of process i
                   finish[i]=1;//if it is done, set finish to 1
                }			 
              }
           }
	   else// if there is one resource not satisfy the request, it is deadlock
	   {
	      flag=1;
	      break;
	   }
        }//for j end
	if(flag)break;//if there is deadlock
	pro_id++;
     }//while end
     

/* --------------display deadlock process----------------- */
     if(flag==1)
     {
         printf("\nSystem is in Deadlock\n\n");
     }
     else
     {
         printf("\nNo Deadlock Occur\n\n");
     }  
}

