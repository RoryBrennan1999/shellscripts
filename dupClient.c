//////////////////////////////////////////////////
// dupClient.c
// A client that implements a full duplex scheme
// Rory Brennan 18237606
//////////////////////////////////////////////////

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "pipe.h" // A common header file for client and server

int main() 
{
	int fdout, fdin, status, numRead;
	int pid = getpid();
	
	// Create message structure
	struct message msg;
	sprintf(msg.messageInfo, "This is client with PID: %d", pid); 
	
	// Create private pipe
	// Convert PID to string using sprintf
	sprintf(msg.privateName, "/tmp/%d", pid); 
	printf("CLIENT: Creating private pipe...\n");
	status = mkfifo(msg.privateName, 0666 | S_IFIFO );
	
	// Check status for any error except an ‘exist’ error
	if (( status == -1) && (errno != EEXIST)) 
	{
		perror("Error creating the private pipe - remove pipe if already exists");
		exit (1);
	}
	
	// Open the public pipe
	printf("CLIENT: Opening public pipe...\n");
	if(( fdout = open( PIPE_1 , O_WRONLY)) == -1) 
	{
		perror(PIPE_1) ;
		exit(1) ;
	}
	
	printf("CLIENT: Sending info over public pipe...\n");
	write( fdout, &msg, sizeof(msg) ) ; // write to the public pipe

	close (fdout) ;
	
	// Open private pipe
	fdin = open(msg.privateName, O_RDONLY);
	printf("CLIENT: Opening private pipe...\n");
	
	// Read structure from pipe	
	numRead = read(fdin, &msg, sizeof(msg)); 
	printf("CLIENT: Reading from private pipe...\n");
	
	if( numRead != 0 )
	{
		printf("CLIENT: Converted Message: %s\n", msg.messageInfo);
	}
	
	printf("CLIENT: Now exiting...\n");
	
	return (0) ;
}