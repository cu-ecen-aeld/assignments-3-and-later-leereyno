#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{
    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    //struct thread_data* thread_func_args = (struct thread_data *) thread_param;

	// Create a pointer to thread_param cast as a pointer to struct thread_data
   struct thread_data* thread_func_args = (struct thread_data *) thread_param;

	// Store the result of our attempt to lock the mutex
	int lockresult;

	// store the result of trying to unlock the mutex
	int unlockresult;

	// We're going to sleep for this many microseconds before getting the lock
	// We multiply by 1000 to convert milliseconds to microseconds
	useconds_t wait_to_obtain_ms = thread_func_args->wait_to_obtain_ms * 1000;

	// We're going to sleep for this many microseconds before releasing the lock
	useconds_t wait_to_release_ms = thread_func_args->wait_to_release_ms * 1000;

	// Store the value of our thread id to be referenced by the calling function
	thread_func_args->mythreadid = pthread_self ();

	// sleep before locking mutex
	usleep(wait_to_obtain_ms);

	// Try to lock the mutex
	lockresult = pthread_mutex_lock( thread_func_args->mutex);

	if ( lockresult != 0 )
	{
		errno = lockresult;
		perror("obtain mutex");

		// Let the calling function know there was a problem
		thread_func_args->thread_complete_success = false;

    	return thread_param;
	}

	// sleep before releasing mutex
	usleep(wait_to_release_ms);

	// Try to unlock the mutex
	unlockresult = pthread_mutex_unlock ( thread_func_args->mutex);

	if ( unlockresult != 0 )
	{
		errno = unlockresult;
		perror("release mutex");    

		// Let the calling function know there was a problem
		thread_func_args->thread_complete_success = false;

    	return thread_param;
	}

	// If we got this far then everything is cool
	thread_func_args->thread_complete_success = true;

	return thread_param;
}

bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */
    //return false;

	// Return value from thread creation step
	int ret;

	// Create a pointer for the stucture data we're going to dynamically allocate
	struct thread_data * this_thread_data;

	// Allocate memory from this_thread_data
	this_thread_data = (struct thread_data *) malloc(sizeof(struct thread_data));

	// Populate our structure with the wait times
	this_thread_data->wait_to_obtain_ms = wait_to_obtain_ms;
	this_thread_data->wait_to_release_ms = wait_to_release_ms;

	// Set the structure mutex pointer to point to the mutex pointer passed into this function
	this_thread_data->mutex = mutex;

	// Try to create our function
	ret = pthread_create(thread, NULL, threadfunc, this_thread_data);

	if ( ret != 0 )
	{
		errno = ret;
		perror("thread create");
		return false;
	}
	else
		return true;
}

