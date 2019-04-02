/*
   ----- 48450 -- week 2 lab handout 2 ------ 

fork() creates a new process by duplicating the calling process. The new process, referred to as the child, is an exact duplicate of the calling process.

On success, the PID of the child process is returned in the parent, and 0 is returned in the child. On failure, -1 is returned in the parent, in which case, no child process is created, and errno is set appropriately.
*/

#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
  pid_t pid;
	/* fork a child process */
	pid=fork();
	
	printf("pid order: pid=%d\n",pid);//to see the pid execute order

	if(pid<0){/* error occurred*/
	  fprintf(stderr, "Fork Failed");
	  return 1;
	}
	else if(pid ==0){/* child process*/
	  printf("Child Begins, to do ls operation, pid=%d\n",pid);

      /* after run the following line, the child is ruturn */
 
	  
 /**********************************************/
 /* 	Please look at the following two code lines

	Qustion 1 -- If you do not want return now, comment the execlp code line. What does the fork run?
    	
	Question 2 -- if you replace the current process with a new process such as printf ("The process is ...). What 	does the fork run?
	
	Question 3 -- if we keep the excelp, why the following 	code line won't run?
    	
*/

	execlp("/bin/ls","ls",NULL); 

     printf("The process is still in the child process, pid=%d\n",pid);

} else //If comment this line, child will both run the following lines, why? 
      {/* parent process:this code line is very interesting to look at*/
	  printf("This is the parent process, pid=%d\n\n",pid);
	  
	  wait(NULL); /* parent will wait for the child to complete*/
	  printf("\nChild Complete and back to Parent \n");
	}
	printf("see what pid is now: pid=%d\n",pid);//to see the pid execute order
	return 0;
}
