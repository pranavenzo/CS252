#ifndef CS252_UTILS_H
#define CS252_UTILS_H

/**
 * Lots of C macro magic going on here...
 * In this file, a bunch of debugging macros are implemented for you:
 * d_print/d_printf, d_warn/d_warnf, and d_error/d_errorf
 * You can call each like a function -- `d_print("Hello world!");`
 * The macros will print the file, line number, and containing function of the print call
 * The difference between print/warn/error is that print shows as green, warn as yellow, and error as red.
 * The difference between d_print and d_printf is that d_print just takes a single char const*, while
 * d_printf takes an initial char const* format string and then additional arguments, just like printf.
 * Because of how macros work, we need two different functions for these situations (ie, you can't
 * call d_printf("Hello world!");, because it will want additional arguments).
 * Finally, the macros are only enabled if NDEBUG is not defined, so you can turn off your debug messages
 * (as well as assert()ions) by defining NDEBUG, likely using `make DFLAG="-DNDEBUG"`
 *
 * Remember, we are not grading the output of your server except for printing the initial config!
 * Use these print functions liberally.
**/

#ifndef NDEBUG
#define DEBUG 1
#else
#define DEBUG 0
#endif

#include <cstdio>
#include <cstdlib>
#include <cstring>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define d_print(str) \
            do { \
                if (DEBUG) fprintf(stderr, ANSI_COLOR_GREEN "%s:%d in %s():" ANSI_COLOR_RESET " %s\n", __FILE__, __LINE__, __func__, str); \
            } while (0)

#define d_printf(fmt, ...) \
            do { \
                if (DEBUG) { \
                    fprintf(stderr, ANSI_COLOR_GREEN "%s:%d in %s(): " ANSI_COLOR_RESET, __FILE__, __LINE__, __func__); \
                    fprintf(stderr, fmt, __VA_ARGS__); \
                    fprintf(stderr, "\n"); \
                } \
            } while (0)

#define d_error(str) \
            do { \
                if (DEBUG) fprintf(stderr, ANSI_COLOR_RED "%s:%d in %s():" ANSI_COLOR_RESET " %s\n", __FILE__, __LINE__, __func__, str); \
            } while (0)

#define d_errorf(fmt, ...) \
            do { \
                if (DEBUG) { \
                    fprintf(stderr, ANSI_COLOR_RED "%s:%d in %s(): " ANSI_COLOR_RESET, __FILE__, __LINE__, __func__); \
                    fprintf(stderr, fmt, __VA_ARGS__); \
                    fprintf(stderr, "\n"); \
                } \
            } while (0)

#define d_warn(str) \
            do { \
                if (DEBUG) fprintf(stderr, ANSI_COLOR_YELLOW "%s:%d in %s():" ANSI_COLOR_RESET " %s\n", __FILE__, __LINE__, __func__, str); \
            } while (0)

#define d_warnf(fmt, ...) \
            do { \
                if (DEBUG) { \
                    fprintf(stderr, ANSI_COLOR_YELLOW "%s:%d in %s(): " ANSI_COLOR_RESET, __FILE__, __LINE__, __func__); \
                    fprintf(stderr, fmt, __VA_ARGS__); \
                    fprintf(stderr, "\n"); \
                } \
            } while (0)

#endif
