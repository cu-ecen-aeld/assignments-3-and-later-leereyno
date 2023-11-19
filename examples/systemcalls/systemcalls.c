#include "systemcalls.h"

#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

//#define FORKDEBUG 1

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{

/*
 * TODO  add your code here
 *  Call the system() function with the command set in the cmd
 *   and return a boolean true if the system() call completed with success
 *   or false() if it returned a failure
*/

	if ( system(cmd) == 0 )
		return true;
	else
		return false;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
			#ifdef FORKDEBUG
			fprintf(stderr,"normal command[%d] = %s\n",i,command[i]);
			#endif
    }
    command[count] = NULL;

    va_end(args);

/*
 * TODO:
 *   Execute a system command by calling fork, execv(),
 *   and wait instead of system (see LSP page 161).
 *   Use the command[0] as the full path to the command to execute
 *   (first argument to execv), and use the remaining arguments
 *   as second argument to the execv() command.
 *
*/
	 
	int ret;								// return value from execv			
	int status;							// status of child process returned from wait() in parent

	pid_t waitforme;					// pid of child process returned from wait() in parent

	pid_t pid;							// used with fork() : Will be >0 in parent and 0 in child

	// Create a child of the current process
	pid = fork();				

	if ( pid == -1 )
	{
		perror("do_exec failure to fork");
		return false;
	}
	else if ( pid > 0 ) // parent
	{
		// Wait for the child process to exit and save its status information
		waitforme = waitpid (pid, &status, 0);
		
		if ( waitforme == -1 )
		{
			perror("parent : waitpid returned error");
			return false;
		}
		else if (WIFEXITED(status)) // Did the process run exit() when terminating?
		{
			#ifdef FORKDEBUG
			fprintf(stderr,"parent : normal WIFEXITED return code = %d\n",WIFEXITED(status));
			fprintf(stderr,"parent : normal WEXITSTATUS return code = %d\n",WEXITSTATUS(status));
			#endif

			if ( WEXITSTATUS(status) == 0) // Did the process return 0 when exiting?
				return true;
			else
				return false;
		}
		// If we don't know how the process ended then assume there was an error
		else
			return false;
	}
	else if ( pid == 0 ) // child
	{

		#ifdef FORKDEBUG
		fprintf(stderr,"child : normal command_zero : %s\n",command[0]);
		#endif

		// Attempt to run our program
		ret = execv(command[0],command);

		if ( ret == -1) // Unable to exec new program
		{
			perror("Exec failure");
			exit(1); // We must use exit(1) to pass this return code to waitpid() in parent
		}

	}

	return true; // Process should never reach this line, but this avoids compile error

}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
		  #ifdef FORKDEBUG
		  fprintf(stderr,"redir command[%d] = %s\n",i,command[i]);
		  #endif
    }
    command[count] = NULL;

    va_end(args);

/*
 * TODO
 *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
 *   redirect standard out to a file specified by outputfile.
 *   The rest of the behaviour is same as do_exec()
 *
*/
	 
	int ret;								// return value from execv			
	int status;							// status of child process returned from wait() in parent

	pid_t waitforme;					// pid of child process returned from wait() in parent

	pid_t pid;							// used with fork() : Will be >0 in parent and 0 in child

	int outfilefd;						// File descriptor for outputfile

	// Create a child of the current process
	pid = fork();				

	if ( pid == -1 )
	{
		perror("do_exec failure to fork");
		return false;
	}
	else if ( pid > 0 ) // parent
	{
		// Wait for the child process to exit and save its status information
		waitforme = waitpid ( pid, &status, 0 );
		
		if ( waitforme == -1 )
		{
			perror("parent : waitpid returned error");
			return false;
		}
		else if (WIFEXITED(status)) // Did the process run exit() when terminating?
		{
			#ifdef FORKDEBUG
			fprintf(stderr,"parent : normal WIFEXITED return code = %d\n",WIFEXITED(status));
			fprintf(stderr,"parent : normal WEXITSTATUS return code = %d\n",WEXITSTATUS(status));
			#endif

			if ( WEXITSTATUS(status) == 0) // Did the process return 0 when exiting?
				return true;
			else
				return false;
		}
		// If we don't know how the process ended then assume there was an error
		else
			return false;
	}
	else if ( pid == 0 ) // child
	{
		// Open the file we'll be redirecting stdout to
		outfilefd = open( outputfile,
						O_WRONLY | O_CREAT | O_TRUNC,
						S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH
					);

		// Error opening file
		if ( outfilefd == -1 )
		{
			perror("Error opening outputfile");
			return false;
		}

		// Close the file descriptor for stdout and make the stdout file descriptor
		// point to outfilefd
		//
		// see : man dup2
		//
		// Anything sent to stdout will now go to our outputfile
		if ( dup2(outfilefd,1) == -1)
		{
			perror("Unable to redirect stdout to new file descriptor");
			return false;
		}

		#ifdef FORKDEBUG
		fprintf(stderr,"child : redir command_zero : %s\n",command[0]);
		#endif

		// Attempt to run our program
		ret = execv(command[0],command);

		if ( ret == -1)
		{
			perror("redir Unable to launch");
			exit(1);
		}
	}

	return true; // Process should never reach this line, but this avoids compile error
}
