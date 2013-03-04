/* file: minunit.h (modified from: http://www.jera.com/techinfo/jtns/jtn002.html) */

#define mu_assert(message, test) \
        do { if (!(test)) { \
                sprintf(minunit_msg, \
				"%s%s:%d ASSERT - %s%s", "\033[0;31m", __FILE__, __LINE__, message, "\033[0m"); \
                return minunit_msg; }\
        } while (0)

#define mu_assert_eq(expected, actual) {\
        long a = (long)(expected); \
        long b = (long)(actual); \
        do { if ((a) != (b)) { \
                sprintf(minunit_msg, \
				"%s%s:%d: ASSERT_EQ - expected %ld, got %ld%s", \
				"\033[0;31m", __FILE__, __LINE__, a, b, "\033[0;31m");\
                return minunit_msg; }; \
        } while(0); }

#define mu_assert_ge(min, actual) {\
        long a = (min); \
        long b = (actual); \
        do { if ((b) < (a)) { \
                sprintf(minunit_msg, \
				"%s%s:%d: ASSERT_GE - expected min %ld, got %ld%s", \
				"\033[0;31m", __FILE__, __LINE__, a, b, "\033[0m");\
                return minunit_msg; }; \
        } while(0); }

#define mu_run_test(test) do { char *message = test(); tests_run++; \
        if (message) return message; } while (0)

#define mu_run_suite(suite) do { char* result = suite(); \
        if (result != 0) {printf("%sFAILURE %s%s\n", "\033[0;31m", result, "\033[0m"); return 1; } } while(0)

extern int tests_run;
char minunit_msg[255];


