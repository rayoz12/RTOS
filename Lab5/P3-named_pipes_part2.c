/*
 *	----- 48450 -- week 5 lab handout 3 part 2------ 
   By programing the handout 3 part 2, you will follow the lecture notes to implement the Named pipe (FIFO). In this handout, we have created 2 processes. the writing process -- P3-named_pipes_part1.c and reading process P3-named_pipes_part2.c. You need to compile the both c files and run them in two linux terminals. Once you open the two terminals, one is run the writing process which you can type the characters and you will see all the characters appear on the reading process terminal.  
*
*/
// This program reads data from FIFO
#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
  
int main() 
{ 
    int fd1; 
  
    // FIFO file path 
    char * myfifo = "/tmp/myfifo"; 
  
    // Creating the named file(FIFO) 
    // mkfifo(<pathname>,<permission>) 
    mkfifo(myfifo, 0666); 
  
    char str1[80], str2[80]; 
    while (1) 
    { 
	printf("Process 2 is waiting for data from FIFO\n");
        // First open in read only and read 
        fd1 = open(myfifo,O_RDONLY); 
        read(fd1, str1, 80); 
  
        // Print the read string and close 
        printf("Read from FIFO: %s\n", str1); 
        close(fd1); 
    } 
    return 0; 
} 
