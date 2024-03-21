#include <stdio.h>
#include <stdlib.h>

#include "utils/fork.h"
#include "utils/get_tokens.h"
#include "utils/preprocess.h"
#include "utils/types.h"

#define test 0

int main(void) {

  run_stat stat = {
      .should_run = 1,  /* flag to determine when to exit program */
      .should_fork = 1, /* fork as default except running built-in command */
      .wait_flag = 1    /* wait by default */
  };

  history_head *histories = (history_head *)malloc(sizeof(history_head));
  histories->n_nodes = 0, histories->first = NULL;

  int command_index = 1;

  while (stat.should_run) {
    printf("o-shell -> ");
    fflush(stdout);

    // Maximum 40 arguments with one command
    char **argv = (char **)calloc(
        sizeof(char *), MAX_LINE / 2 + 1); /* command line arguments */

    // (0) read user input
    const int argc = get_tokens(argv);
    // if there is no token, just continue
    if (argc == 0) {
      // no input, thus the command index wont change
      continue;
    }

    // Before forking a new process, a preprocessor is needed
    // this preprocesser can call shell built-in functions
    stat = preprocess(argc, argv, histories, &command_index);
    if (stat.should_run == 0) {
      break;
    }
    // built-in command finished
    if (stat.should_fork == 0) {
      command_index++;
      continue;
    }

#if test
    printf("All %d tokens:\n", argc);
    char *const *ptr = argv;
    while (ptr && *ptr != NULL) {
      printf("%s\n", *ptr);
      ptr++;
    }
#endif

    command_index++;
    fork_new_process(argv, stat.wait_flag);
  }

  free_all_histories(histories);

  return 0;
}