/*
   ----- 48450 -- week 2 lab handout 3------ 

To illustrate the concept of cooperating processes -- IPC, let us consider the producer-consumer problem, which is a common paradigm for cooperating processes. A producer process produces information that is consumed by a consumer process

Shared memory-1

*/
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


#define BUFFER_SIZE 15
#define TRUE 1

char *text="Jian Zhang OS Code";

typedef struct
{
  char elem;
}item;

int main()
{
  item buffer[BUFFER_SIZE];
  int in=0;
  int out=0;

  item next_produced, next_comsumed;
  while(TRUE)
  {
	printf("Begin produce:\n");
	while(1)
	{
	    if(((in+1)%BUFFER_SIZE)==out)
		{
		    printf("\nFull.\n");break;
		}//do nothing 
 
	    next_produced.elem=text[in]; // What is code line doing? -- assign the text to next_produced!
	    buffer[in]=next_produced;
	    printf("%c",next_produced.elem); // What this code line doing?
	    //printf("\n"); 
		in=(in+1)%BUFFER_SIZE;
	}

	printf("\nBegin comsume:\n");
	while(TRUE)
	{
	    if(in==out)
	    {
		printf("\nComsume complete, it is empty now.\n");break;
	    }//do nothing 

	    next_comsumed=buffer[out];
  	    printf("%c",next_comsumed.elem);//What is code line doig?
	    out=(out+1)%BUFFER_SIZE;
	    //comsume the item in next consumed, What is the code line doing?
	}
	

	break;
  }
  
  
}


