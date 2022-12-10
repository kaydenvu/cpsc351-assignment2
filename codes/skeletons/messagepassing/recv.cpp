#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>
using namespace std;
// The name of the shared memory segment
#define MSQ_NAME "/cpsc351queue"

// The name of the file where to save the received file
#define RECV_FILE_NAME "file__recv"

#define MQ_MSGSIZE 4096


/**
 * Receive the file from the message queue 
 */
void recvFile()
{
	// TODO: Define a data structure
	// of type mq_attr to specify a
	// queue that can hold up to 10
	// messages with the maximum message
	// size being 4096 bytes
	struct mq_attr attr;
  attr.mq_flags = 0;  
  attr.mq_maxmsg = 10;  
  attr.mq_msgsize = MQ_MSGSIZE;  
  attr.mq_curmsgs = 0;
	
	// The buffer used to store the message copied
	// copied from the shared memory
	char buff[MQ_MSGSIZE];
	
	// The total number of bytes written
	int totalBytesWritten = 0;

	// The number of bytes written 
	int bytesWritten = 0;
	
	// The return value
	int msqBytesRecv = -1;
	
	// Open the file for writing
	int fd = open(RECV_FILE_NAME, O_CREAT | O_WRONLY | O_TRUNC, 0777);
	
	// Make sure the file was opened
	if(fd < 0)
	{
		perror("open");
		exit(1);
	}


	
	// TODO: Create the message queue
	// whose name is defined by the macro
	// MSQ_NAME macro above. 
	// The queue should be configured with
	// parameters defined by the data 
	// message queue data structure defined above.
	// The queue should have permissions of 0600.
  
  mqd_t queue = mq_open(MSQ_NAME, O_CREAT|O_RDWR, 0600, &attr);


	while(msqBytesRecv != 0)
	{
    cout << queue << endl;
    msqBytesRecv = mq_receive(queue, buff, sizeof(buff), NULL);
    cout << msqBytesRecv << endl;
    if (msqBytesRecv < 0) {
      perror("receive");
      exit(1);
    }
    else if (msqBytesRecv != 0) {
      bytesWritten = write(fd, buff, msqBytesRecv);
      if (bytesWritten < 0) {
        perror("write");
        exit(1);
      }
    }
    else {
      exit(0);
    }
		// TODO: Receive the message.
		// Write the data from the message
		// into the file (represented by the 
		// descriptor fd). Please note:
		// the mq_receive() function
		// you will use will return
		// the size of the message
		// received (in bytes).
		// If the size of the message
		// is 0, it is the sender telling
		// us that it has no more data to send.
		// Please see the sender's code for the
		// corresponding logic
	}
	
	// TODO: Deallocate the message queue	
  mq_close(fd);
}


int main(int argc, char** argv)
{	
	
	recvFile();
		
	return 0;
}
