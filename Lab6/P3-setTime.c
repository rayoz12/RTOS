/*
----- 48450 -- week 6 lab handout 3 ------ 
By programing the handout 3, you will follow the lecture notes to implement the clokc set up.

This program sets the clock forward 1 day. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main( void )
  {
    struct timespec stime;
	/* get the current system time*/
    if( clock_gettime( CLOCK_REALTIME, &stime) == -1 ) {
       perror( "getclock" );
       exit( EXIT_FAILURE );
    }
    time_t now;
    time(&now);
    printf("Current time is: %s\n", ctime(&now));
	
    stime.tv_sec += (60*60)*24L;  /* Add one day */
    stime.tv_nsec = 0;
	/*set the system time into a new time*/
    if( clock_settime( CLOCK_REALTIME, &stime) == -1 ) {
       perror( "setclock" );
       exit( EXIT_FAILURE );
    }

    time(&now);
    printf("After changing the time, current time is: %s\n", ctime(&now));
    return( EXIT_SUCCESS );
  }

