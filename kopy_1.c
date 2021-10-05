/******************************************************************
kopy_1.c
An example Unix-like shell copy command/utility.
Rory Brennan 18237606
*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h> // For write() and read()
#include <ctype.h> // For tosupper() and islower()
#include <sys/time.h> // For gettimeofday()

#define BUF_LEN 1024 // Macro definition for buffer length

int main (int argc, char *argv[]) 
{
	int i, j;
	int uppercase_flag, size_flag, rate_flag = 0; // Flags for different command options
	long int buff_iterations = 0; // Counter for size of file
	long int start, end = 0; // Start/End points for time
	long int elapsed_time = 0; // Elapsed time in microseconds
	float rate = 0; // Rate of transfer
	struct timeval startVal, endVal; // Length of time for copying
	int fileArg = 0; // Count for files on command line
	int infd, outfd; // File descriptor for input and output file
	int num_read, num_write; // Numbers of characters returned by a file access
	char buff [BUF_LEN]; // Buffer for use in file access
	char* fileName[2] = {} ; // Point to file names
	
	// If statement to check if required number of arguments are present
	if ( argc < 4 && argc >= 3) // Check for missing commands
	{
		printf("Please choose a command option....\n");
		printf("-u: Convert characters to uppercase\n");
		printf("-s: Output size of file in KB\n");
		printf("-r: Output copy time in microseconds and rate of transfer in MB/s\n");
	} else if ( argc < 2 ) // Check for missing files & commands
	{
		printf("Please choose a command option....\n");
		printf("-u: Convert characters to uppercase\n");
		printf("-s: Output size of file in KB\n");
		printf("-r: Output copy time in microseconds and rate of transfer in MB/s\n");
		
		printf("Error: Missing input and destination file!\n");
	} else if ( argc < 3 && argc >= 2 ) // Check for missing destination file
	{
		printf("Please choose a command option....\n");
		printf("-u: Convert characters to uppercase\n");
		printf("-s: Output size of file in KB\n");
		printf("-r: Output copy time in microseconds and rate of transfer in MB/s\n");
		
		printf("Error: Missing destination file!\n");
	}
	
	// Parse the command line arguments
	for ( i = 1 ; i < argc; i++ ) 
	{
		if (argv[i][0] == '-') // Check for a command option
		{
			switch(argv[i][1]) 
			{
				case 'u':
					// Set flag for conversion
					printf ("Converting characters to Uppercase....\n");
					uppercase_flag = 1;
					// no break; for fall through (-usr)
				case 's':
					// Set flag for counting size
					size_flag = 1;
				case 'r':
					// Set flag for rate of transfer
					rate_flag = 1;
					// Begin tracking time
					gettimeofday(&startVal, NULL);
			}
	
		} else fileName[fileArg++] = argv[i];
	}

	// Open the first file for reading
	infd = open (fileName[0], O_RDONLY);
	if (infd == -1)
	{ 
		printf ("Failed to open first file %s\n", fileName[0]);
		return 1;
	}

	// Open/create the second file for writing
	outfd = open (fileName[1], O_CREAT | O_WRONLY, 0600 );
	if (outfd == -1)
	{ 
		printf ("Failed to open second file %s\n", fileName[1]);
		close (infd);
		return 1;
	}
	
	// Now copy the contents of the first file to the second file
	while ( ( num_read = read (infd, buff, sizeof (buff)) ) > 0 )
	{
		// If uppercase_flag is set then convert all buff's characters to upper case
		if ( uppercase_flag == 1 )
		{ 	
			for ( i=0; i<sizeof (buff); i++ )
				if ( islower (buff[i]) )
				buff[i] = toupper ( buff[i] );
		}
		
		num_write = write (outfd, buff, num_read);

		// Check for basic failure
		if (num_write != num_read){
			printf ("Problem - mismatch between write size and read size!\n");
		}
		
		// Iterate buff counter
		buff_iterations++;
	}
	
	// Remember to close all open files before exit
	close (outfd);
	close (infd);
	
	// Make sure buffer iterations are correct for memory output
	// If buffer length is 4096, multiply iterations by 4 
	if ( BUF_LEN == 4096 )
	{
		buff_iterations = buff_iterations * 4;
	}
	
	// Check for size flag
	if ( size_flag == 1 )
	{
		// Print out size of file in KB
		printf ("Size of file: %ld KB\n", buff_iterations);
	}
	
	// Check for rate flag
	if ( rate_flag == 1 )
	{
		// Calculate elapsed time
		gettimeofday(&endVal, NULL);
		// Convert to microseconds and subtract initial time from final time
		start = startVal.tv_sec * 1000000 + startVal.tv_usec;
		end = endVal.tv_sec * 1000000 + endVal.tv_usec;
		elapsed_time = (end - start);
		// Print out elapsed time and rate of transfer
		printf ("Elapsed Time: %ld us\n", elapsed_time);
		// Compute rate, convert to MB/s and print
		rate = ( (float) buff_iterations / (float) elapsed_time);
		printf ("Rate of Transfer: %f MB/s\n", rate*1000);
	}
	
	return 0 ;
}