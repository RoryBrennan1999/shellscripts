//////////////////////////////////////////////////
// dupServer.c
// A server that implements a full duplex scheme
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
#include <ctype.h> // For isupper() and islower()
#include "pipe.h" // A common header file for client and server

int main() 
{
	printf("======================================\n");
	printf("       Server booting up.......       \n");
	printf("           CTRL + C to exit			  \n");
	printf("======================================\n\n");
	
	int i, fdin, fdout, status, count, numRead ;
	struct message who;
	
	// Create named pipe with read and write access
	status = mkfifo(PIPE_1, 0666);
	
	// Check status for any error except an ‘exist’ error
	if (( status == -1) && (errno != EEXIST)) 
	{
		perror("Error creating the named pipe - remove pipe if already exists");
		exit (1);
	}
	
	printf("SERVER: Public pipe created sucessfully!\n");
	printf("SERVER: Attempting to read from a public pipe....\n\n");
	
	while(1)
	{
		fdin = open(PIPE_1, O_RDONLY); // Open public pipe for reading
		numRead = read(fdin, &who, sizeof(who)); // Read structure from pipe
		
		if( numRead != 0 )
		{
			printf("SERVER: Got from public pipe: %s\n", who.messageInfo);
			printf("SERVER: Converting message to upper case....\n");
			
			// Convert all characters to upper case
			for ( i = 0; i < sizeof(who.messageInfo); i++ ) 
			{
				if ( islower (who.messageInfo[i]))
					who.messageInfo[i] = toupper (who.messageInfo[i]);
			}
		}
		
		close(fdin);
		
		// Open private pipe
		fdout = open(who.privateName, O_WRONLY);
		if(( fdout = open( who.privateName, O_WRONLY)) == -1) 
		{
			perror(who.privateName) ;
			exit(1) ;
		}
		
		// Write to private pipe
		printf("SERVER: Sending converted info over private pipe @ %s...\n", who.privateName);
		write( fdout, &who, sizeof(who) ) ; // write to the private pipe
		
		close(fdout);
		
		printf("SERVER: Waiting for incoming connections....\n\n");
		
		sleep(1);
	}
	unlink(PIPE_1);
}