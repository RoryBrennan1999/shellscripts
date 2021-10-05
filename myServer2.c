//////////////////////////////////////////////////////////////////////////////////////////////////////////
// myServer2.c
// Rory Brennan 18237606
// Implements a multitasking server scheme using threads.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Here are the necessary includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

// A counter global variable
int counter; 
	
// Initialise the mutex and give it the name ‘lock’
// This will be used to avoid a race on the global counter variable
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER ;

// threadFunction - threads will call this function.
void *threadFunction( void *connFd );

// MAIN
int main() 
{
	pthread_t thread; // Create a thread for our clients
	int rc;
	struct sockaddr_in servAddr; // declare socket address structure
	int connFd, sockFd; // socket descriptors, behave like file descriptors
	
	memset(&servAddr, 0, sizeof(servAddr)); // clear the socket address structure
	
	// Create an unnamed socket ... name will be binded in later
	sockFd = socket(PF_INET, SOCK_STREAM, 0); // Note PF_, not AF_
	if(sockFd == -1) 
	{
		perror("did not create the socket");
		exit(EXIT_FAILURE); 
	}
	
	// Fill in the address structure
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(5000);
	
	if (inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr) == -1) 
	{
		perror("inet_pton problem");
		exit(EXIT_FAILURE); 
	}
	
	// Name the socket by binding it to the servAddr structure's data
	if ( bind(sockFd, (struct sockaddr*)&servAddr, sizeof(servAddr)) == -1 )
	{
		perror("did not bind socket properly");
		close (sockFd);
		exit(EXIT_FAILURE); 
	}
	
	// Set up 'listen' queue for say 6 client connections
	if ( listen(sockFd, 6) == -1 ) 
	{
		perror("listen failure");
		close (sockFd);
		exit(EXIT_FAILURE); 
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	// Server is now set up, print a starting message from server
	printf ("\nI am the server starting up. Hit Ctrl C to get rid of me!\n");
	printf ("\nServer info: (IP: %s, port: %d)\n\n", inet_ntoa(servAddr.sin_addr),
	ntohs(servAddr.sin_port));
	////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Now – continuously loop accepting and processing client requests
	while(1) 
	{
		connFd = accept(sockFd, (struct sockaddr*)NULL, NULL); // wait for client and process it
		if ( connFd < 0) 
		{
			perror("accept call failed");
			close(connFd);
			close (sockFd);
			exit(EXIT_FAILURE); 
		}
		
		// Create thread
		if ( (rc=pthread_create (&thread, NULL, &threadFunction, (void*) &connFd )))
		{
			printf ("Error in creating thread %d\n", rc); 
		}
		
		// The 'pthread_join' function causes main() to wait until a thread is properly created
		pthread_join ( thread, NULL) ;
		
		printf ("I have just dealt with a client!\n"); // message to the user
		
	} // end of while() loop
		
	close (sockFd); // Close the socket
	pthread_mutex_destroy (&lock) ; // Destroy the mutex on exit
	return (EXIT_SUCCESS);
	
} // end of main()

// Thread function
void *threadFunction( void *connFd ) 
{
	// Cast connection ID that is passed to an int
	int conFd = *(int*)connFd;
	
	// use some buffers for messages
	char transmitBuff[1024] = {0};
	char receiveBuff[1024] = {0}; 
	
	pthread_mutex_lock (&lock) ; // Lock the critical section with the mutex
	counter++;
		
	// Fill transmitter buffer and clear the receiver buffer
	sprintf(transmitBuff, "Hello client, the server is using this fd for you: %d\n", conFd) ;
	memset(receiveBuff, 0, sizeof(receiveBuff)); // clear the receive buffer
			
	// Read from the connection
	if ( ( read(conFd, receiveBuff, sizeof(receiveBuff) ) ) == -1 ) 
	{
		perror("read from connection failure");
		close (conFd);
		exit(EXIT_FAILURE); 
	}
			
	printf ("This server to date has an aggregate of %d reads for all clients\n", counter);
	printf ("I just read this from a client: %s\n", receiveBuff);
			
	// Write to the connection
	if ( ( write(conFd, transmitBuff, strlen(transmitBuff)) ) == -1 ) 
	{
		perror("write to connection failure");
		close (conFd);
		exit(EXIT_FAILURE); 
	}
			
	// If the received string is "End" than close the connection for this client
	if (strcmp( receiveBuff, "End\r\n") == 0 ) 
	{
		printf ("Closing down this client connection\n");
		close(conFd); // Close the connection
	}
	
	pthread_mutex_unlock (&lock) ; // Unlock the critical section with the mutex 
	pthread_exit (NULL) ; // thread exits
}
