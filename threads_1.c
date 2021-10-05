/********************************************************************************
Program: threads_1.c

A simple threaded program example where each one of three threads carefully writes to a
defined section of the console using a semaphore-based solution for synchronisation.

Rory Brennan 18237606
*******************************************************************************/

// Header files
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <pthread.h>

// Functions - separate threads will call these functions.
void *functionA() ;
void *functionB() ;
void *functionC() ;
void *functionD() ;

// Declare semaphore
sem_t mutex_A ;

// main function
int main () 
{
	sem_init(& mutex_A,0,1); // Initialise console semaphore to 1
	pthread_t thread1, thread2, thread3, thread4; // Declare four thread IDs
	int rc1, rc2, rc3, rc4 ; 

	// Here the console screen is described in four sections
	printf ("\033[2J") ; // Clear the screen
	printf ("\033[%d;%dH", 0, 0); // Set cursor position ( row 0, column 0)
	printf ("THREAD ONE\n");
	printf ("===========================================================\n");
	printf ("\033[%d;%dH", 8, 0 ); // Set cursor position ( row 8, column 0)
	printf ("THREAD TWO \n");
	printf ("===========================================================\n");
	printf ("\033[%d;%dH", 16, 0); // Set cursor position ( row 16, column 0)
	printf ("THREAD THREE \n");
	printf ("===========================================================\n");
	printf ("\033[%d;%dH", 21, 0); // Set cursor position ( row 21, column 0)
	printf ("THREAD FOUR \n");
	printf ("===========================================================\n");
	
	// Create first thread ... thread1
	if ( (rc1=pthread_create (&thread1, NULL, &functionA, NULL )))
	{
		printf ("Error in creating thread %d\n", rc1); 
	}
	
	// Create second thread ... thread2
	if ( (rc2=pthread_create (&thread2, NULL, &functionB, NULL )))
	{ 
		printf ("Error in creating thread %d\n", rc2); 
	}
	
	// Create third thread ... thread3
	if ( (rc3=pthread_create (& thread3, NULL, &functionC, NULL )))
	{ 
		printf ("Error in creating thread %d\n", rc3); 
	}
	
	// Create fourth thread ... thread4
	if ( (rc4=pthread_create (& thread4, NULL, &functionD, NULL )))
	{ 
		printf ("Error in creating thread %d\n", rc4); 
	}
	
	// The 'pthread_join' function causes main() to wait until a thread is properly created
	pthread_join ( thread1, NULL) ;
	pthread_join ( thread2, NULL) ;
	pthread_join ( thread3, NULL) ;
	pthread_join ( thread4, NULL) ;
	
	sem_destroy ( & mutex_A ) ; // Destroy the semaphore on exit
	exit (0); // exit the main function
}

// This is “thread1”, uses functionA()
void *functionA() 
{
	while (1) 
	{
		sem_wait ( & mutex_A ) ; // do a semaphore wait on mutex_A 
		printf ("\033[%d;%dH", 3, 0) ; // Set cursor position ( row 3, column 0)
		printf ("I am function 111111111111111111111111111111111111111111\n");
		sem_post ( & mutex_A ) ; // do a semaphore post (i.e. signal) on mutex_A 
	}
	pthread_exit (NULL) ; // thread exits
}

// This is “thread2”, uses functionB()
void *functionB() 
{
	while (1) 
	{
		sem_wait ( & mutex_A ) ; // do a semaphore wait on mutex_A 
		printf ("\033[%d;%dH", 11, 0) ; // Set cursor position ( row 11, column 0)
		printf ("I am function 222222222222222222222222222222222222222222\n") ;
		sem_post ( & mutex_A ) ; // do a semaphore post (i.e. signal) on mutex_A
	} 
	pthread_exit (NULL) ; // thread exits
}

// This is “thread3”, uses functionC()
void *functionC() 
{
	while (1) 
	{
		sem_wait ( & mutex_A ) ; // do a semaphore wait on mutex_A 
		printf ("\033[%d;%dH", 19, 0) ; // Set cursor position ( row 19, column 0)
		printf ("I am function 333333333333333333333333333333333333333333\n") ;
		sem_post ( & mutex_A ) ; // do a semaphore post (i.e. signal) on mutex_A
	}
	pthread_exit (NULL) ; // thread exits
}

// This is “thread4”, uses functionD()
void *functionD() 
{
	while (1) 
	{
		sem_wait ( & mutex_A ) ; // do a semaphore wait on mutex_A 
		printf ("\033[%d;%dH", 24, 0) ; // Set cursor position ( row 24, column 0)
		printf ("Rory Brennan 18237606\n") ;
		sem_post ( & mutex_A ) ; // do a semaphore post (i.e. signal) on mutex_A
	}
	pthread_exit (NULL) ; // thread exits
}