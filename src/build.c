
/**
 * @file /magma/engine/status/build.c
 *
 * @brief	Functions for retrieving the version and build information.
 */

#include "magma.h"

#ifndef CORE_VERSION
#error The core library version is missing.
#endif

#ifndef CORE_COMMIT
#error The core library commit identity is missing.
#endif

#ifndef CORE_TIMESTAMP
#error The core library timestamp is missing.
#endif

/**
 * @brief	Get the core library version string.
 * @return	a pointer to a null-terminated string containing the core library version string.
 */
const char * build_version(void) {
	return (const char *)CORE_VERSION;
}

/**
 * @brief	Get the  core library commit identity.
 * @return	a pointer to a null-terminated string containing the last eight characters of current commit identity.
 */
const char * build_commit(void) {
	return (const char *)CORE_COMMIT;
}

/**
 * @brief	Get the core library build stamp.
 * @return	a pointer to a null-terminated string containing the core library build information string.
 */
const char * build_stamp(void) {
	return (const char *)CORE_TIMESTAMP;
}

