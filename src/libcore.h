
/**
 * @file /libcore/libcore.h
 *
 * @brief The global include file. This header includes both system headers
        and the core module headers.
 */

#ifndef LIBCORE_H
#define LIBCORE_H

#ifndef __USE_GNU
#define __USE_GNU
#endif

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
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/utsname.h>
#include <sys/prctl.h>
#include <sys/epoll.h>
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

#if defined(linux) || defined(__linux) || defined(__linux__)
#include <execinfo.h>
#endif

#include "core/core.h"

#define CORE_SECURE_MEMORY_ENABLED true
#define CORE_SECURE_MEMORY_LENGTH 65536
#define CORE_THREAD_STACK_SIZE 1048576
#define CORE_PAGE_LENGTH getpagesize()
#define MAGMA_FILEPATH_MAX PATH_MAX

extern __thread char threadBuffer[1024];
#define bufptr (char *)&(threadBuffer)
#define buflen sizeof(threadBuffer)

static inline bool_t status(void) {
	return true;
}

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

/// log.c
void   log_disable(void);
void   log_enable(void);

/// build.c
const   char * build_commit(void);
const   char * build_stamp(void);
const   char * build_version(void);

#endif
