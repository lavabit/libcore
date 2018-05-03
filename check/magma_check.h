
/**
 * @file /check/magma/magma_check.h
 *
 * @brief The entry point for accessing the modules involved with executing unit tests.
 */

#ifndef FAKE_MAGMA_CHECK_H
#define FAKE_MAGMA_CHECK_H

#include "magma.h"

#include <ctype.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <check.h>
#include <valgrind/valgrind.h>

// Normally the START_TEST macro creates a static testcase function. Unfortunately we can't
// find those symbols using dlsym() so we can't dynamically select individual test cases at
// runtime. This redefines the macro without using the static keyword to workaround this problem.
#undef START_TEST
#define START_TEST(__testname) void __testname (int _i CK_ATTRIBUTE_UNUSED) {  tcase_fn_start (""# __testname, __FILE__, __LINE__);

#include "core/core_check.h"

extern int case_timeout;

#define log_unit(...) log_internal ( __VA_ARGS__)
//#define testcase(s, tc, name, func) tcase_add_test((tc = tcase_create(name)), func); tcase_set_timeout(tc, case_timeout); suite_add_tcase(s, tc)

Suite * suite_check_sample(void);
void log_test(chr_t *test, stringer_t *error);
void suite_check_testcase(Suite *s, const char *tags, const char *name, TFun func);

#define status() (true)
#define thread_start() (true)
#define thread_stop() do {} while(0)
// log.c
void     log_disable(void);
void     log_enable(void);
void log_internal(const char *format, ...);

#undef log_pedantic
#undef log_check
#undef log_info
#undef log_error
#undef log_critical
#undef log_options


// Macro used record debug information during development.
#define log_pedantic(...) log_internal (__VA_ARGS__)

// Log an error message if the specified conditional evaluates to true.
#define log_check(expr) do { if (expr) log_internal (__STRING (expr)); } while (0)


// Used to record information related to daemon performance.
#define log_info(...) log_internal (__VA_ARGS__)

// Used to log errors that may indicate a problem requiring user intervention to solve.
#define log_error(...) log_internal (__VA_ARGS__)

// Used to record errors that could cause system instability.
#define log_critical(...) log_internal (__VA_ARGS__)

// Used to override the globally configured log options for a specific entry.
#define log_options(options, ...) log_internal (__VA_ARGS__)


#endif

//! Quick Test
#if 1
#define RUN_TEST_CASE_TIMEOUT 100
#define PROFILE_TEST_CASE_TIMEOUT 1000

#define INX_CHECK_MTHREADS 2
#define INX_CHECK_OBJECTS 1024

#define IP_CHECK_ROUNDS 10

#define TREE_INSERTS_CHECK 128
#define TREE_CURSORS_CHECK 128
#define LINKED_INSERTS_CHECK 128
#define LINKED_CURSORS_CHECK 128
#define HASHED_INSERTS_CHECK 128
#define HASHED_CURSORS_CHECK 128

#define QP_CHECK_SIZE 1024
#define URL_CHECK_SIZE 1024
#define HEX_CHECK_SIZE 1024
#define BASE64_CHECK_SIZE 1024
#define ZBASE32_CHECK_SIZE 1024
#define CHECKSUM_CHECK_SIZE 1024

#define QP_CHECK_ITERATIONS 16
#define URL_CHECK_ITERATIONS 16
#define HEX_CHECK_ITERATIONS 16
#define BASE64_CHECK_ITERATIONS 16
#define ZBASE32_CHECK_ITERATIONS 16
#define CHECKSUM_CHECK_ITERATIONS 16

#define TANK_CHECK_DATA_HNUM 1L
#define TANK_CHECK_DATA_UNUM 1L
#define TANK_CHECK_DATA_MTHREADS 2 // Disabled
#define TANK_CHECK_DATA_CLEANUP true

#define DSPAM_CHECK_SIZE_MIN 1024
#define DSPAM_CHECK_SIZE_MAX (2 * 1024)
#define DSPAM_CHECK_DATA_UNUM 1L
#define DSPAM_CHECK_ITERATIONS 128

// Controls the size of the compression test block.
#define COMPRESS_CHECK_SIZE_MIN 1024 // 1 kilobyte
#define COMPRESS_CHECK_SIZE_MAX (2 * 1024) // 2 kilobytes
#define COMPRESS_CHECK_MTHREADS 2 // Disabled
#define COMPRESS_CHECK_ITERATIONS 16

#define RAND_CHECK_SIZE_MIN 64
#define RAND_CHECK_SIZE_MAX 128
#define RAND_CHECK_ITERATIONS 128
#define RAND_CHECK_MTHREADS 2

#define SCRAMBLE_CHECK_SIZE_MIN (1024) // 1 kilobyte
#define SCRAMBLE_CHECK_SIZE_MAX (2 * 1024) // 2 kilobytes
#define SCRAMBLE_CHECK_ITERATIONS 16

#define DIGEST_CHECK_SIZE 1024
#define DIGEST_CHECK_ITERATIONS 16

#define SYMMETRIC_CHECK_SIZE_MIN 64
#define SYMMETRIC_CHECK_SIZE_MAX (2 * 1024) // 2 kilobytes
#define SYMMETRIC_CHECK_ITERATIONS 16

#define ECIES_CHECK_SIZE_MIN 64
#define ECIES_CHECK_SIZE_MAX (2 * 1024) // 2 kilobytes
#define ECIES_CHECK_ITERATIONS 16

#define PRIME_CHECK_SIZE_MIN 64
#define PRIME_CHECK_SIZE_MAX (2 * 1024) // 2 kilobytes
#define PRIME_CHECK_ITERATIONS 16
#define PRIME_CHECK_SPANNING_CHUNK_SIZE (1024 * 1024 * 20) // 20 megabytes

#define OBJECT_CHECK_ITERATIONS 16

#define MAIL_CHECK_LOAD_MAX 64 // Maximum number of messages loaded per user.

#define REGRESSION_CHECK_FILE_DESCRIPTORS_LEAK_MTHREADS 8

//! Exhaustive Test
#else

// Maximum number of seconds for a given test case.
#define RUN_TEST_CASE_TIMEOUT 86400
#define PROFILE_TEST_CASE_TIMEOUT 864000

#define INX_CHECK_MTHREADS 8
#define INX_CHECK_OBJECTS 8192

#define TREE_INSERTS_CHECK 8192
#define TREE_CURSORS_CHECK 8192
#define LINKED_INSERTS_CHECK 8192
#define LINKED_CURSORS_CHECK 8192
#define HASHED_INSERTS_CHECK 8192
#define HASHED_CURSORS_CHECK 8192

#define QP_CHECK_SIZE 8192
#define URL_CHECK_SIZE 8192
#define HEX_CHECK_SIZE 8192
#define BASE64_CHECK_SIZE 8192
#define ZBASE32_CHECK_SIZE 8192
#define CHECKSUM_CHECK_SIZE 8192

#define QP_CHECK_ITERATIONS 8192
#define URL_CHECK_ITERATIONS 8192
#define HEX_CHECK_ITERATIONS 8192
#define BASE64_CHECK_ITERATIONS 8192
#define ZBASE32_CHECK_ITERATIONS 8192
#define CHECKSUM_CHECK_ITERATIONS 8192

#define TANK_CHECK_DATA_HNUM 1L
#define TANK_CHECK_DATA_UNUM 1L
#define TANK_CHECK_DATA_MTHREADS 8
#define TANK_CHECK_DATA_CLEANUP true

#define DSPAM_CHECK_DATA_UNUM 1L
#define DSPAM_CHECK_ITERATIONS 8192
#define DSPAM_CHECK_SIZE_MIN 1024
#define DSPAM_CHECK_SIZE_MAX (16 * 1024)
//#define DSPAM_CHECK_SIZE_MAX (1 * 1024 * 1024) // 1 megabyte

#define COMPRESS_CHECK_MTHREADS 8
#define COMPRESS_CHECK_ITERATIONS 256
#define COMPRESS_CHECK_SIZE_MIN 1024 // 1 kilobyte
#define COMPRESS_CHECK_SIZE_MAX (16 * 1024)
//#define COMPRESS_CHECK_SIZE_MAX (1 * 1024 * 1024) // 1 megabyte

#define RAND_CHECK_MTHREADS 8
#define RAND_CHECK_ITERATIONS 256
#define RAND_CHECK_SIZE_MIN 1024 // 1 kilobyte
#define RAND_CHECK_SIZE_MAX (16 * 1024)
//#define RAND_CHECK_SIZE_MAX (1 * 1024 * 1024) // 1 megabyte

#define ECIES_CHECK_ITERATIONS 256
#define ECIES_CHECK_SIZE_MIN 1024 // 1 kilobyte
#define ECIES_CHECK_SIZE_MAX (16 * 1024)
//#define ECIES_CHECK_SIZE_MAX (1 * 1024 * 1024) // 1 megabyte

#define PRIME_CHECK_ITERATIONS 256
#define PRIME_CHECK_SIZE_MIN (1024) // 1 kilobyte
#define PRIME_CHECK_SIZE_MAX (1 * 1024 * 1024) // 1 megabyte
#define PRIME_CHECK_SPANNING_CHUNK_SIZE (1024 * 1024 * 256) // 256 megabytes

#define SCRAMBLE_CHECK_ITERATIONS 256
#define SCRAMBLE_CHECK_SIZE_MIN 1024 // 1 kilobyte
#define SCRAMBLE_CHECK_SIZE_MAX (16 * 1024)
//#define SCRAMBLE_CHECK_SIZE_MAX (1 * 1024 * 1024) // 1 megabyte

#define DIGEST_CHECK_ITERATIONS 256
#define DIGEST_CHECK_SIZE (16 * 1024)
//#define DIGEST_CHECK_SIZE (1 * 1024 * 1024) // 1 megabyte

#define SYMMETRIC_CHECK_ITERATIONS 256
#define SYMMETRIC_CHECK_SIZE_MIN 1024 // 1 kilobyte
#define SYMMETRIC_CHECK_SIZE_MAX (16 * 1024)
//#define SYMMETRIC_CHECK_SIZE_MAX (1 * 1024 * 1024) // 1 megabyte

#define OBJECT_CHECK_ITERATIONS 256

#define MAIL_CHECK_LOAD_MAX UINT64_MAX // Maximum number of messages loaded per user.

#define REGRESSION_CHECK_FILE_DESCRIPTORS_LEAK_MTHREADS 32

#endif
