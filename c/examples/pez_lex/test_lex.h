#include <stdio.h>
#include <string.h>
#include "minunit.h"
#include "lex_fsm.h"


char msg[50]; // for when you want to sprintf stuff


static char* test_simple_words() {
	char *input = "monkey (smurf) bar \\|baz|";

	
	lex(&input);
	sprintf(msg, "Expected 'monkey', got '%s'", token_buffer);
	mu_assert(msg, strcmp(token_buffer, "monkey") == 0);

	lex(&input);
	sprintf(msg, "Expected 'bar', got '%s'", token_buffer);
	mu_assert(msg, strcmp(token_buffer, "bar") == 0);
	
  //lex(&input);
  //sprintf(msg, "Expected 'baz', got '%s'", token_buffer);
  //mu_assert(msg, strcmp(token_buffer, "baz") == 0);
	
	return 0;
}

static char* test_EOS() {
	char *input = "monkey bar";
	
	lex(&input);
	sprintf(msg, "Expected 'monkey', got '%s'", token_buffer);
	mu_assert(msg, strcmp(token_buffer, "monkey") == 0);

	lex(&input);
	sprintf(msg, "Expected 'bar', got '%s'", token_buffer);
  int test_value = strcmp(token_buffer, "bar");
	mu_assert(msg, test_value == 0);
	
	return 0;
}

char* lexing_suite() {
	
	define_trans_table();

	mu_run_test(test_simple_words);
	mu_run_test(test_EOS);

	return 0;
}

