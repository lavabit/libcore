
/**
 * @file /libcore/libcore.h
 *
 * @brief The global include file. This header includes both system headers
        and the core module headers.
 */

#ifndef LIBCORE_H
#define LIBCORE_H

#define __USE_GNU

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <signal.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <pthread.h>
#include <fcntl.h>
#include <math.h>
#include <semaphore.h>
#include <dirent.h>
#include <limits.h>
#include <ftw.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/utsname.h>
#include <sys/resource.h>

//from magma.h
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stddef.h>
#include <limits.h>
#include <signal.h>
#include <string.h>
#include <dirent.h>
#include <pwd.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <execinfo.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/utsname.h>
#include <sys/prctl.h>
#include <sys/epoll.h>
#include <sys/sysctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <arpa/nameser.h>
#include <netdb.h>
#include <resolv.h>
#include <regex.h>
#include <ftw.h>
#include <search.h>
#include <semaphore.h>
#include <sys/mman.h>

// GNU C Library
#include <gnu/libc-version.h>

#include "core/core.h"

/// random.c
int16_t    rand_get_int16(void);
int32_t    rand_get_int32(void);
int64_t    rand_get_int64(void);
int8_t     rand_get_int8(void);
uint16_t   rand_get_uint16(void);
uint32_t   rand_get_uint32(void);
uint64_t   rand_get_uint64(void);
uint8_t    rand_get_uint8(void);
bool_t     rand_start(void);
void       rand_stop(void);
size_t     rand_write(stringer_t *s);

extern __thread char threadBuffer[1024];
#define bufptr (char *)&(threadBuffer)
#define buflen sizeof(threadBuffer)

static inline bool_t status(void) {
	return true;
}

/**
 *
 * @brief	Logs the message described by format, and provided as a variadic argument list.
 * @param 	format	The printf style format for the log message.
 * @param	va_list	A variadic list of data items to be used by the format string.
 * @return	This function returns no value.
 */
static inline void log_internal(const char *format, ...) {

	va_list args;

	va_start(args, format);

	mutex_lock(&log_mutex);

	// Someone has disabled the log output.
	if (!log_enabled) {
		mutex_unlock(&log_mutex);
		return;
	}

	vfprintf(stdout, format, args);
	fprintf(stdout, "\n");

	fflush(stdout);
	mutex_unlock(&log_mutex);

	va_end(args);

	return;
}

/**
 * @brief	Disable logging.
 * @return	This function returns no value.
 */
static inline log_disable(void) {
	mutex_lock(&log_mutex);
	log_enabled = false;
	mutex_unlock(&log_mutex);
	return;
}

#endif
