/*
   ----- 48450 -- week 2 lab handout 3 ------ 

To illustrate the concept of cooperating processes -- IPC, let us consider the producer-consumer problem, which is a common paradigm for cooperating processes. A producer process produces information that is consumed by a consumer process

A Shared-Memory Example for Producer
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h> /* For O_* constants */ 

int main()
{
 /* the size (in bytes) of shared memory object */
  const int SIZE = 4096;
 /* NAME OF THE SHARED MEMORY OBJECT */
  const char *name = "OS";
 /* strings written to shared memory */
  const char *message_0="Hello";
  const char *message_1="World\n";

 /* shared memory file descriptor */
  int shm_fd;
 /* pointer to shared memory object */
  void *ptr;

        /* What is the following code line doing? */
	shm_fd = shm_open(name,O_CREAT|O_RDWR, 0666);

	/* to configure the size of the shared memory object -- why we need this? */
	ftruncate(shm_fd,SIZE);

	/* What is the following code line doing? */
	ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

	/* What is following code lines doing? write to the shared memory object */
	sprintf(ptr,"%s",message_0);
	ptr += strlen(message_0);
	sprintf(ptr,"%s",message_1);

	return 0;
}

/* when do you comile this program please type - gcc P5-shareMemory-producer.c -o P5 -lrt 
rt -- real time library
*/
