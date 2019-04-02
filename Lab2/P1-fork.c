/*
   ----- 48450 -- week 2 lab handout 1 ------ 

fork() creates a new process by duplicating the calling process. The new process, referred to as the child, is an exact duplicate of the calling process.

On success, the PID of the child process is returned in the parent, and 0 is returned in the child. On failure, -1 is returned in the parent, in which case, no child process is created, and errno is set appropriately.
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

void do_child_things()
{
  printf("This is a child process\n");
  printf("Do child things\n");
}

void do_parent_things()
{
  printf("This is a parent process\n");
  printf("Do parent things\n");
}

void main()
{
	pid_t pid;
	switch(pid=fork())
	{
	case -1:
	  perror("fork:");	
	  return;
	case 0:
	  do_child_things();
	  break;
	default:
	  printf("child pid=%d, parent's=%d\n",(int)pid,(int) getpid());
	  wait(NULL); //if uncomment this line, the parent exit without waiting child process
	  do_parent_things();
	  break;
	}
}

