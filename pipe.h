//////////////////////////////////////////////////
// pipe.h
// A header file for dupServer.c and dupClient.c
// Rory Brennan 18237606
//////////////////////////////////////////////////

#define PIPE_1 "/tmp/pipe_1"
#define BUF_SIZE 256

struct message {
	char messageInfo[80] ;
	char privateName[20] ;
};
