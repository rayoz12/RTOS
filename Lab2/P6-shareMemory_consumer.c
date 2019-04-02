/*
   ----- 48450 -- week 2 lab handout 4------ 

To illustrate the concept of cooperating processes -- IPC, let us consider the producer-consumer problem, which is a common paradigm for cooperating processes. A producer process produces information that is consumed by a consumer process

A Shared-Memory Example for Consumer
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

 /* shared memory file descriptor */
  int shm_fd;
 /* pointer to shared memory object */
  void *ptr;

        /* What is the following code line doing? -- create the shared memory object */
	shm_fd = shm_open(name,O_RDONLY, 0666);

	ftruncate(shm_fd,SIZE);

	/* What is the following code line doing? map files into a shared memory */
	ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

	/* What is following code line doing? */
	printf("%s",(char*)ptr);

	/* What is the following code line doing?  */
	shm_unlink(name);

	return 0;

}

/* when do you comile this program please type - gcc P6-shareMemory-consumer.c -o P6 -lrt --real time library */
