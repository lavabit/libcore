
/**
 * @file /libcore/src/log.c
 *
 * @brief Logging functions.
 */

#include "magma.h"

bool_t log_enabled = true;
pthread_mutex_t log_mutex =	PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief	Disable logging.
 * @return	This function returns no value.
 */
void log_disable(void) {
	mutex_lock(&log_mutex);
	log_enabled = false;
	mutex_unlock(&log_mutex);
	return;
}

/**
 * @brief	Enable logging.
 * @return	This function returns no value.
 */
 void log_enable(void) {
	mutex_lock(&log_mutex);
	log_enabled = true;
	mutex_unlock(&log_mutex);
	return;
}
