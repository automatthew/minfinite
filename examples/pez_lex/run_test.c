#include "test_lex.h"
/*#include "minunit.h"*/

int tests_run = 0;


int main(int argc, char **argv) {
	mu_run_suite(lexing_suite);

	printf("%sALL TESTS PASSED. %d tests run.%s\n", "\033[0;32m", tests_run, "\033[0m");

	return 0;
}

