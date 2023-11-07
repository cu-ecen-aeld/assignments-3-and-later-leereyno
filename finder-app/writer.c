#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <syslog.h>

#define BUFFERSIZE 4096

/***************************************************************************
Function : send_to_syslog

Summary:

   Sends a message to syslog

Variables:
   priority : The log level
   message : The actual message sent to syslog

***************************************************************************/
void send_to_syslog(int priority, char * message)
{
	openlog(NULL,LOG_PID,LOG_USER);
	syslog(priority, "%s", message);

	closelog();
}

int main(int argc, char * argv[])
{
	// VARIABLES:
	
	// A string buffer that we'll populate and send to syslog when there are errors
	char logbuffer[BUFFERSIZE];

	// Create a file descriptor for our output file
	int fd;

	// Many library calls set the errno variable
	// Create a local variable to save the value of this variable for later use
	int localerrno;

	// How long is our input string?  Used with the write() call
	ssize_t string_length = 0 ;

	// How much data was written to our output file?
	ssize_t written = 0;

	// Status variable to check that a flush to disk succeeds
	int fsync_status = 0;

	// Make sure both of our command line arguments are specified
	if ( argc != 3 )
	{
		printf("\nOne or both arguments are missing\n");
		return 1;
	}

	// Understanding program arguments:
	//
	// argv[0] is the path to the running program (e.g. ./writer )
	// argv[1] is our first argument, the path to the file we will be creating
	// argv[2] is the string we'll be writing into the file in argv[1]

	// Create more descriptive aliases for our command line arguments
	const char * filename = argv[1];
	const char * stringout = argv[2];

	// How long is the string we're writing out to our file?
	string_length = strlen(stringout);

	// Open our file for writing:
	//
	// Per the instructions, we can assume that the directory where our file lives already exists
	// so we can just open this file for writing:

	fd = open( filename,
	           O_WRONLY | O_CREAT | O_TRUNC,
	           S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH
	         );

	// flags:
	//
	// O_WRONLY = Open file for writing
	// O_CREAT = Create file if it doesn't exist
	// O_TRUNC = If file does exist, truncate the size to zero ( aka remove its contents )

	// file permissions:
	// S_IWUSR = Owner can write to file
	// S_IRUSR = Owner can read from file
	//
	// S_IWGRP = Group can write to file
	// S_IRGRP = Group can read from file
	//
	// S_IROTH = Everyone else can read from file


	// Did something go wrong?
	if ( fd == -1)
	{
		localerrno = errno;  // Save errno since library calls might change its value. Best practice
		// populate logbuffer with an error message, send it to the log, and print it to the screen
		//	
		snprintf(logbuffer,BUFFERSIZE,"Error opening file \"%s\" : %s\n",filename,strerror(localerrno));
		send_to_syslog(LOG_ERR,logbuffer);
		printf("%s",logbuffer);
		return 1;
	}

	// Send a non-error message to our logfile saying that we're going to write to our output file
	snprintf(logbuffer,BUFFERSIZE,"Writing %s to %s",stringout,filename);
	send_to_syslog(LOG_DEBUG,logbuffer);

	// Write to our output file, and save how many bytes were actually written
	written = write ( fd, stringout, string_length	);

	if ( written == -1 ) // Unable to write
	{
		localerrno = errno;
		snprintf(logbuffer,BUFFERSIZE,"Error writing to file \"%s\" : %s\n",filename,strerror(localerrno));
		send_to_syslog(LOG_ERR,logbuffer);
		printf("%s",logbuffer);
		return 1;
	}
	else if ( written != string_length ) // Could not write everything we wanted to
	{
		snprintf(logbuffer,BUFFERSIZE,"Only %ld bytes from string of %ld length written\n",(long int)written,(long int)string_length);
		send_to_syslog(LOG_ERR,logbuffer);
		printf("%s",logbuffer);
		return 1;
	}

	// Flush buffers to disk
	fsync_status = fsync ( fd );

	if ( fsync_status == -1 )
	{
		localerrno = errno;
		snprintf(logbuffer,BUFFERSIZE,"Error flushing file \"%s\" to disk : %s\n",filename,strerror(localerrno));
		send_to_syslog(LOG_ERR,logbuffer);
		printf("%s",logbuffer);
		return 1;
	}

	// Close our file
	if ( close ( fd ) == -1 )
	{
		localerrno = errno;
		snprintf(logbuffer,BUFFERSIZE,"Error closing file \"%s\" : %s\n",filename,strerror(localerrno));
		send_to_syslog(LOG_ERR,logbuffer);
		printf("%s",logbuffer);
		return 1;
	}

	return 0;
}
