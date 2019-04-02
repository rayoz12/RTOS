/*
   ----- 48450 -- week 4 lab handout 2 ------ 
By programing the handout 1, you can try to add the semaphore to control the critical section of two threads' operation: if the runnerOne is running, the runnerTwo should wait. 
*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

int sum;/*this data is shared by the threads*/

/* the semaphores */
sem_t one, two;
pthread_t tid1,tid2;       //Thread ID
pthread_attr_t attr; //Set of thread attributes

void *runnerOne(void *param);/*threads call this function*/
void *runnerTwo(void *param);/*threads call this function*/
void initializeData();

int main(int argc, char*argv[])
{
  int flag; /* the flag to set the sem_post to lead the runnerOne or runnerTwo thread */

  if(argc!=2){
	fprintf(stderr,"usage: ./P2 number  \n<note: an integer value for loop counting times & the even value will set runnerOne go first and vice versa for the odd value>\n");
	return -1;
  }
  if(atoi(argv[1])<0){
	fprintf(stderr,"%d must be >=0\n",atoi(argv[1]));
	return -1;
  }
  initializeData();

  /*get the default attributes*/
  pthread_attr_init(&attr);
  /*create the thread*/
  pthread_create(&tid1,&attr,runnerOne,argv[1]);

  printf("sum=%d\n",sum);

  /*create the thread*/
  pthread_create(&tid2,&attr,runnerTwo,argv[1]);

  flag=atoi(argv[1])%2;
  switch (flag){
	case 0: sem_post(&one); break;
	case 1: sem_post(&two); break;
	default: printf("Invalid swtich\n" );
  }

	  	 
  /*wait for the thread to exit*/
  pthread_join(tid1,NULL);
  pthread_join(tid2,NULL);

  /* why sum values are different if we set 'one' first and vice versa ? */  
  printf("sum=%d\n",sum);

}

/*The thread will begin control in this function*/
void *runnerOne(void *param)
{
  /* aquire the full lock */
  sem_wait(&one);

  int i,upper=atoi(param);
  
  
  for(i=0;i<=upper;i++)
    sum+=2*i;

  printf("thread one\n");

  /* signal empty */
  sem_post(&two);

}

/*The thread will begin control in this function*/
void *runnerTwo(void *param)
{
  /* aquire the full lock */
  sem_wait(&two);

  int i,upper=atoi(param);
  
  for(i=0;i<=upper;i++)
    sum+=i;
  printf("thread two\n");

  /* signal empty */
  sem_post(&one);
}

void initializeData() {
   sum=0;

   /* Create the full semaphore and initialize to 0 */
   sem_init(&one, 0, 0);

   /* Create the empty semaphore and initialize to BUFFER_SIZE */
   sem_init(&two, 0, 0);

   /* Get the default attributes */
   pthread_attr_init(&attr);
}
