/*********************************************************************
Program: deadlockExist.c
A simple threaded program to demonstrate the deadlock condition.
Rory Brennan 18237606
********************************************************************/
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

//Function declarations
void *functionA() ;
void *functionB() ;
int var_A, var_B = 0 ; // The shared variables

//Declare mutex
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

// MAIN
int main() 
{
	pthread_t thread1, thread2 ; // declare 2 threads
	int rc1, rc2 ;
	
	// Message to the terminal
	printf ("\033[2J") ; // Clear the screen
	printf ("Deadlock experiment with MUTEX. Does this program lead to deadlock?\n") ;
	printf ("=============================================================\n\n") ;
	
	// Create first thread
	if ((rc1=pthread_create(&thread1, NULL, &functionA, NULL))) 
	{
		printf("Error in creating %d\n",rc1) ; 
	}
	// Create next thread
	if ((rc2=pthread_create(&thread2, NULL, &functionB, NULL))) 
	{
		printf("Error in creating %d\n",rc2) ; 
	}
	
	// Remember to use the thread joins
	pthread_join( thread1, NULL) ;
	pthread_join( thread2, NULL) ;
	pthread_mutex_destroy(&mutex1) ;
	pthread_mutex_destroy(&mutex2) ;
	// main exits
	
	exit(0) ;
}

// Thread 1
void *functionA() 
{
	int x = 0 ; // interation counter for the While loop
	while (x < 10000) 
	{
		pthread_mutex_lock(&mutex1) ;
		printf ("I'm T1, just locked mutex1\n") ;
		pthread_mutex_lock(&mutex2) ;
		printf ("I'm T1, just locked mutex2\n") ;
		var_A++ ;
		var_B++ ;
		printf ("I am Thread 1: var_A = %d, var_B = %d \n", var_A, var_B) ;
		pthread_mutex_unlock(&mutex2) ;
		printf ("I'm T1, just freed mutex2\n") ;
		pthread_mutex_unlock(&mutex1) ;
		printf ("I'm T1, just freed mutex1\n") ;
		x++ ;
	}
	pthread_exit (NULL) ; // Thread exits
}

// Thread 2
void *functionB() 
{
	int x = 0 ; // interation counter
	while (x < 10000) 
	{
		pthread_mutex_lock(&mutex2) ;
		printf ("I'm T2, just locked mutex2\n") ;
		pthread_mutex_lock(&mutex1) ;
		printf ("I'm T2, just locked mutex1\n") ;
		var_A++ ;
		var_B++ ;
		printf("I am Thread 2: var_A = %d, var_B = %d \n", var_A, var_B) ;
		pthread_mutex_unlock(&mutex1) ;
		printf ("I'm T2, just freed mutex1\n") ;
		pthread_mutex_unlock(&mutex2) ;
		printf ("I'm T2, just freed mutex2\n") ;
		x++ ;
	}
	pthread_exit (NULL) ; // Thread exits
}