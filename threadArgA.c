/////////////////////////////////////////////////////////////////////////////////
// Program: threadArgA.c
//
// Demonstrates passing an argument to a new thread and reporting on execution time.
//
// Rory Brennan 18237606
/////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

void *my_function (void*); // Function for the thread

// Declare ru as a struct rusage;
struct rusage ru; 

// MAIN
int main ()
{
	// Report on CPU time when program is beginning
	getrusage(RUSAGE_SELF, &ru); 
	printf ("Usage results at beginning of program:\n");
	printf("User space CPU time used: %2lus %6luus\n", ru.ru_utime.tv_sec, ru.ru_utime.tv_usec );
	printf("Kernel CPU time used: %6lus %6luus\n\n", ru.ru_stime.tv_sec, ru.ru_stime.tv_usec );
	
	pthread_t my_thread ; // Declare a thread identifier
	int rc1, x = 7;
	
	// Create my_thread
	if ( (rc1 = pthread_create (& my_thread, NULL, & my_function, (void*) &x)))
	{ 
		printf ("Error in creating thread %d\n", rc1) ; 
	}
	
	pthread_join ( my_thread, NULL); // wait for thread to exit
	return (0); // exit the main function
}

/////////////////////////////////////////////////////////////////////////////////
// The my_thread is created with my_function() which accepts an argument
/////////////////////////////////////////////////////////////////////////////////
void *my_function(void* arg)
{
	int i = *(int*)arg;
	
	// Report on CPU time when thread is created
	getrusage(RUSAGE_SELF, &ru); 
	printf ("Usage results at beginning of thread:\n");
	printf ("The argument which this thread received is %d \n", i ) ;
	printf("User space CPU time used: %2lus %6luus\n", ru.ru_utime.tv_sec, ru.ru_utime.tv_usec );
	printf("Kernel CPU time used: %6lus %6luus\n\n", ru.ru_stime.tv_sec, ru.ru_stime.tv_usec );
	
	// while loop to decrement i and report execution time
	while ( i > 0) 
	{ 
		getrusage(RUSAGE_SELF, &ru); 
		printf ("Integer i has the value %d \n", i ); 
		printf ("Usage results for this program so far are:\n");
		printf("User space CPU time used: %2lus %6luus\n", ru.ru_utime.tv_sec, ru.ru_utime.tv_usec );
		printf("Kernel CPU time used: %6lus %6luus\n\n", ru.ru_stime.tv_sec, ru.ru_stime.tv_usec );
		i--; 
	}
	
	pthread_exit (NULL) ; // thread exits
}