legacy: pez_char_fsm.c
	gcc -Wall pez_char_fsm.c -o pez_char_fsm

test_lex: lex_fsm.h test_lex.h run_test.c
	gcc -Wall run_test.c -o test_lex -I../include

test: test_lex
	./test_lex

