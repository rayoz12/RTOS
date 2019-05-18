/***********************
   ----- 48450 -- week 4 lab handout 4 ------ 
This is a program for you to excesie the 'signal' of Unix/Linux OS to do an action 'Ctrl-C'.
Singal ID 2 -- Defualt Action is terminate and correspoding event is interrupt (e.g., Ctl-c from keyboard
**********************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void sighandler(int);

int main () {
   /* declare a signal for Ctrl+c (SIGINT: signal interrupt signal)*/
   signal(SIGINT, sighandler);
   
   /*loop until there is a interrupt signal come in*/
   while(1) {
      printf("Going to sleep for a second...\n");
      sleep(1); 
   }
   return(0);
}

void sighandler(int signum) {
/* question: what is the signum? why this number always the same */
   printf("\n Caught signal Ctrl+c, signal number is %d. Coming out...\n",signum);
   exit(1);
}
