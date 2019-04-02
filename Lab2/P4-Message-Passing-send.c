#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>

#define MSGSZ     128


/*
 * Declare the message structure.
 */

typedef struct msgbuf {
         long    mtype;
         char    mtext[MSGSZ];
         } message_buf;

main()
{
    int msqid;
    int msgflg = IPC_CREAT | 0666;
    key_t key;
    message_buf sbuf;// store message
    size_t buf_length;

/* Acquire a string from command line*/
   char second_chs[100];
   printf("please input a string for passing message\n");
   fgets(second_chs, sizeof(second_chs), stdin);   

  /*
     * Get the message queue id for the
     * "name" 1234, which was created by
     * the server.
     */
    key = 1234;

    fprintf(stderr, "\nmsgget: Calling msgget(%#lx,\%#o)\n", key, msgflg);
  
    if ((msqid = msgget(key, msgflg )) < 0) {
        perror("msgget");
        exit(1);
    }
    else 
       fprintf(stderr,"msgget: msgget succeeded: msqid = %d\n", msqid);


    /*
     *  build a message
     */
     
    sbuf.mtype = 1;
     
    strcpy(sbuf.mtext, second_chs);
     
    buf_length = strlen(sbuf.mtext) + 1 ;

    /*
     * Send a message.
     */
    if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0) {
       printf ("%d, %d, %s, %d\n", msqid, sbuf.mtype, sbuf.mtext, buf_length);
       perror("msgsnd");
       exit(1);
    }

   else 
       printf("Message: %s--Sent\n", sbuf.mtext);
      
    exit(0);
}
