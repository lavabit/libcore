
#include "magma_check.h"

Suite * suite_check_magma(void) {
  Suite *s = suite_create("\n\tMagma");
  return s;
}

int main(int argc, char *argv[]) {
	SRunner *sr;
	//int_t failed = 0;
	//time_t test_start, test_end;

	// Unit Test Config
	sr = srunner_create(suite_check_magma());


	srunner_add_suite(sr, suite_check_core());

	// Execute
	log_unit("--------------------------------------------------------------------------\n");

	//log_disable();
	//test_start = time(NULL);
	srunner_run_all(sr, CK_SILENT);
	//test_end = time(NULL);
	//log_enable();

	// Output timing.
	//log_unit("--------------------------------------------------------------------------\n");
	//log_unit("%-63.63s %9lus\n", "TEST DURATION:", test_end - test_start);

	// Summary
	//log_unit("--------------------------------------------------------------------------\n");
	//failed = srunner_ntests_failed(sr);
	//srunner_print(sr, CK_NORMAL);

	// The Check Output Ending
	//log_unit("--------------------------------------------------------------------------\n");

	// Cleanup and free the resources allocated by the check code.
	srunner_free(sr);

	//exit((failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE);

}
