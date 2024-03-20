#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "utils/fork.h"
#include "utils/get_tokens.h"

#define test 1

// define cd function
// char cwd[1024];
// getcwd(cwd, sizeof(cwd));
// printf("Current working directory is: %s\n", cwd);

int main(void) {

  int should_run = 1; /* flag to determine when to exit program */

  // Maximum 40 arguments with one command
  char **argv = (char **)calloc(sizeof(char *),
                                MAX_LINE / 2 + 1); /* command line arguments */

  while (should_run) {
    printf("o-shell -> ");
    fflush(stdout);

    // (0) read user input
    const int argc = get_tokens(argv);
    // if there is no token, just continue
    if (argc == 0) {
      continue;
    }

    // Before forking a new process, a preprocessor is needed

    // built-in command list

    // 1. exit: no need to run anymore, exit normally
    if (strcmp(argv[0], "exit") == 0) {
      should_run = 0;
      continue;
    }

    // check if the input token contains"&"
    int is_equal = (strcmp(argv[argc - 1], "&") == 0);
    int wait_flag = 1; // default: need to wait
    // if last_token == "&",  no need to wait
    if (is_equal) {
      wait_flag = 0;
      // also, this symbol should not be passed as a argument to the new process
      // remove the last argument
      argv[argc - 1] = NULL;
    }

#if test
    printf("All %d tokens:\n", argc);
    char *const *ptr = argv;
    while (ptr && *ptr != NULL) {
      printf("%s\n", *ptr);
      ptr++;
    }
#endif

    fork_new_process(argv, wait_flag);
  }

  // free the input argv
  for (int i = 0; i < MAX_LINE / 2 + 1; ++i) {
    free(argv[i]);
  }
  free(argv);

  return 0;
}