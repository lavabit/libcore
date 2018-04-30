
/**
 * @file /magma/engine/log/log.c
 *
 * @brief	Internal logging functions. This function should be accessed using the appropriate macro.
 */

#include "magma.h"

uint64_t log_date;
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
void log_internal(const char *file, const char *function, const int line, M_LOG_OPTIONS options, const char *format, ...) {
	return;
}
