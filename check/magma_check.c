
#include "magma_check.h"

bool_t log_enabled = true;
pthread_mutex_t log_mutex =	PTHREAD_MUTEX_INITIALIZER;

void log_internal(const char *format, ...) {

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
inline void log_disable(void) {
	mutex_lock(&log_mutex);
	log_enabled = false;
	mutex_unlock(&log_mutex);
	return;
}

/**
 * @brief	Enable logging.
 * @return	This function returns no value.
 */
inline void log_enable(void) {
	mutex_lock(&log_mutex);
	log_enabled = true;
	mutex_unlock(&log_mutex);
	return;
}


/**
 * @file /check/magma/magma_check.c
 *
 * @brief	The unit test executable entry point.
 */

#include "magma_check.h"

int_t case_timeout = RUN_TEST_CASE_TIMEOUT;
bool_t do_virus_check = true, do_tank_check = true, do_dspam_check = true, do_spf_check = true;
chr_t *barrister_unit_test = NULL;

/**
 * @brief Enable the log so we can print status information. We're only concerned with whether the
		test passed or failed. The actual error message will be recorded by the check library, and
		then printed en masse when all the test cases have concluded.
 * @param test	The NULL terminated string showing the suite, test, and threading information.
 * @param error	The error string, which should be NULL if the test was skipped, or if the test passed.
 */
void log_test(chr_t *test, stringer_t *error) {

	log_enable();

	if (!status() || (st_populated(error) && !st_cmp_ci_eq(NULLER("SKIPPED"), error))) {
		log_unit("%-64.64s%s%10.10s%s\n", test, color_yellow_bold(), "SKIPPED", color_reset());
	}
	else if (st_populated(error)) {
		log_unit("%-64.64s%s%10.10s%s\n", test, color_red_bold(), "FAILED", color_reset());
	}
	else {
		log_unit("%-64.64s%s%10.10s%s\n", test, color_green(), "PASSED", color_reset());
	}

	return;
}

void suite_check_testcase(Suite *s, const char *tags, const char *name, TFun func) {

	TCase *tc = NULL;

	tcase_add_test((tc = tcase_create(name)), func);
	tcase_set_timeout(tc, case_timeout);
	suite_add_tcase(s, tc);
	return;
}

Suite * suite_check_single(void) {
  Suite *s = suite_create("\n\tCore");
  return s;
}

/***
 * @return Will return -1 if the code is unable to determine whether tracing is active, 0 if tracing is disabled and
 *		1 if tracing has been detected.
 */
int_t running_on_debugger(void) {

	int_t status, ret;
	pid_t pid, parent;

	if ((pid = fork()) == -1) {
		return -1;
	}

	// If were the child, we'll try to start tracing the parent process. If our trace request fails, we assume that means
	// its already being traced by a debugger.
	else if (pid == 0) {

		parent = getppid();

		if (ptrace(PTRACE_ATTACH, parent, NULL, NULL) == 0) {
			waitpid(parent, NULL, 0);
			ptrace(PTRACE_CONT, NULL, NULL);
			ptrace(PTRACE_DETACH, getppid(), NULL, NULL);
			exit(0);
		}

		exit(1);
	}
	else if ((ret = waitpid(pid, &status, 0)) == pid && WIFEXITED(status) == true) {

		// Use a ternary to guarantee only two possible return values.
		return WEXITSTATUS(status) ? 1 : 0;
	}

	return -1;
}

int main(int argc, char *argv[]) {

	SRunner *sr;
	int_t failed = 0;
	time_t prog_start, test_start, test_end;

	// Setup
	prog_start = time(NULL);

	// Unit Test Config
	sr = srunner_create(suite_check_single());
	srunner_add_suite(sr, suite_check_sample());
	srunner_add_suite(sr, suite_check_core());

	// If were being run under Valgrind, we need to disable forking and increase the default timeout.
	// Under Valgrind, forked checks appear to improperly timeout.
	if (RUNNING_ON_VALGRIND == 0 && (failed = running_on_debugger()) == 0) {
		log_unit("Not being traced or profiled...\n");
		srunner_set_fork_status (sr, CK_FORK);
		case_timeout = RUN_TEST_CASE_TIMEOUT;
	}

	// Trace detection attempted was thwarted.
	else {
		if (failed == -1) log_unit("Trace detection was thwarted.\n");
		else log_unit("Tracing or debugging is active...\n");
		srunner_set_fork_status (sr, CK_NOFORK);
		case_timeout = PROFILE_TEST_CASE_TIMEOUT;
	}

	// Execute
	log_unit("--------------------------------------------------------------------------\n");

	log_disable();
	test_start = time(NULL);
	srunner_run_all(sr, CK_SILENT);
	test_end = time(NULL);
	log_enable();

	// Output timing.
	log_unit("--------------------------------------------------------------------------\n");
	log_unit("%-63.63s %9lus\n", "TEST DURATION:", test_end - test_start);
	log_unit("%-63.63s %9lus\n", "TOTAL DURATION:", test_end - prog_start);

	// Summary
	log_unit("--------------------------------------------------------------------------\n");
	failed = srunner_ntests_failed(sr);
	srunner_print(sr, CK_NORMAL);

	// The Check Output Ending
	log_unit("--------------------------------------------------------------------------\n");

	// Cleanup and free the resources allocated by the check code.
	srunner_free(sr);

	exit((failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE);

}
