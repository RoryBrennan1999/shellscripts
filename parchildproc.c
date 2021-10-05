/******************************************************************
parchildproc.c
Performs basic commands on the UNIX process model
Rory Brennan 18237606
*****************************************************************/

// Header Files
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h> 					// For wait()

// main function
int main ()
{
	int cpid,status;
	int pid = fork () ; 				// Fork a new process, original continues to run
	
	// CHILD PROCESS
	if (pid == 0 ) 						// pid of zero defines the new child process
	{ 
		 printf ("I am child: %d, my parent is: %d \n", getpid(), getppid ());
		 printf ("Priority Adjustment: %d \n", nice(5)); // Set priority to 5
		 if ( getppid() == 1 )
		 {
			 printf("I am an orphan\n");
			 printf ("I am child: %d, my parent is: %d \n", getpid(), getppid ());
		 }
	} 
	else
	{
		printf ("I am parent: %d, my child is: %d \n", getpid(), pid);
		cpid = wait(&status);
		printf ("Child process PID: %d terminated (for safe exit of parent)\n", cpid ) ;
	}
	
	exit(0);
}