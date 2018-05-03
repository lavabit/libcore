
/**
 * @file /magma/engine/status/status.c
 *
 * @brief	Functions used to coordinate system state and worker thread operation and shutdown..
 */

#include "magma.h"

struct {
	pid_t pid;
	uint64_t startup;
} process = {
	.pid = 0,
	.startup = 0
};

int status_level = 0;
pthread_rwlock_t status_lock = PTHREAD_RWLOCK_INITIALIZER;

/**
 * @brief	Check to see if a worker thread should continue processing.
 * @note	This function should be called periodically by worker threads.
 * @return	true if the caller should continue processing (status level is positive) or false otherwise.
 */
bool_t status(void) {
	return true;
}
