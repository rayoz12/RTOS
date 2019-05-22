/* 
* RTOS Autumn 2019
* Assignment 3 Program_2 template
*
*/

/**
 * @file Assignment3_template_Prg_2.c
 * @author Ryan Pereira 98112939
 * @brief 
 * @version 0.1
 * @date 2019-05-22
 * 
 * This program is an implementation of the FIFO Virtual memory page replacement algorithim. 
 */

/*
	Building and Running the program is facilitated by a makefile which outputs to Assignment3_template_Prg_2.out:
	make runPg2
	OR
	make buildPg2,
	./Assignment3_template_Prg_2.out 4
	OR
	gcc -g -Wall Assignment3_template_Prg_2.c -o Assignment3_template_Prg_2.out -pthread
	./Assignment3_template_Pg_2.out 4
*/
/*
Usage:
	Pass in the time quantum and output file arguments as shown below.
	./Assignment3_template_Prg2.out frameSize
	Example:
	./Assignment3_template_Prg2.out 4
*/

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

//Number of pagefaults in the program
int pageFaults = 0;

//Function declaration
void SignalHandler(int signal);

/**
 Main routine for the program. In charge of setting up threads and the FIFO.

 @param argc Number of arguments passed to the program.
 @param argv array of values passed to the program.
 @return returns 0 upon completion.
 */
int main(int argc, char* argv[])
{
	//Register Ctrl+c(SIGINT) signal and call the signal handler for the function.
	//add your code here
	signal(SIGINT, SignalHandler);
	
	if (argc < 2)
	{
		printf("\
Usage: \n\
	Pass in the frame size arguments as shown below. \n\
	./Assignment3_template_Prg2.out frameSize \n\
	Example:\n\
	./Assignment3_template_Prg2.out 4 \n");
		exit(0);
	}

    int i;
	// reference number
	int REFERENCESTRINGLENGTH=24;
	//Argument from the user on the frame size, such as 4 frames in the document
	int frameSize = atoi(argv[1]);
	//Frame where we will be storing the references. -1 is equivalent to an empty value
	uint frame[REFERENCESTRINGLENGTH];
	//Reference string from the assignment outline
	int referenceString[24] = {7,0,1,2,0,3,0,4,2,3,0,3,0,3,2,1,2,0,1,7,0,1,7,5};
	//Next position to write a new value to.
	int nextWritePosition = 0;
	//Boolean value for whether there is a match or not.
	bool inFrame = false;
	//Current value of the reference string.
	int currentValue;


	//Initialise the empty frame with -1 to simulate empty values.
	for(i = 0; i < frameSize; i++)
	{
		frame[i] = -1;
	}

	//Loop through the reference string values.
	for(i = 0; i < REFERENCESTRINGLENGTH; i++)
	{
		//add your code here
		currentValue = referenceString[i];
		/**
		 * FIFO Algo 
		 * Check if the current value is in the frame.
		 * 	if it is continue on like nothing happened
		 * 	if not
		 * 		increment page fault count
		 * 		Write to frame (based on nextWritePosition)
		 * 		update nextWritePosition (check if greater than frame size then increment)
		 * 		
		 */
		
		
		//check if frame has the value.
		inFrame = false;
		int j;
		for (j = 0; j < frameSize; j++)
		{
			if (currentValue == frame[j]) {
				inFrame = true;
				break;
			}
		}


		//If it isn't in the frame we have a page fault and place it at the oldest point in the array
		if (!inFrame)
		{
			pageFaults++;
			frame[nextWritePosition] = currentValue;
			nextWritePosition = nextWritePosition + 1 >= frameSize ? 0 : nextWritePosition + 1;
		}
	}

	printf("Waiting for Signal Interrupt\n");

	//Sit here until the ctrl+c signal is given by the user.
	while(1)
	{
		sleep(1);
	}

	return 0;
}

/**
 Performs the final print when the signal is received by the program.

 @param signal An integer values for the signal passed to the function.
 */
void SignalHandler(int signal)
{
	printf("\nTotal page faults: %d\n", pageFaults); //account for starting from 0
	exit(0);
}
