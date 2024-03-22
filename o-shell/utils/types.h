#ifndef SHELL_TYPES_H
#define SHELL_TYPES_H

#include <stddef.h>
#include <stdlib.h>

#define MAX_LINE 80          /* The maximum length in rows */
#define MAX_INPUT_SIZE 1024  /* max input size is 1024 chars*/
#define MAX_BUFFER_SIZE 1024 /* Buffer Size */

/*                run_stats definition and different run_stats                */
typedef struct run_stat {
  int should_run;
  int should_fork;
  int wait_flag;
} run_stat;

// default behavior
#define FORK_AND_WAIT                                                          \
  (run_stat) { 1, 1, 1 }

// detecting '&', no wait
#define FORK_AND_NO_WAIT                                                       \
  (run_stat) { 1, 1, 0 }

// run built-in functions
#define RUN_BUILTIN_COMMAND                                                    \
  (run_stat) { 1, 0, 0 }

// run exit()
#define EXIT_STAT                                                              \
  (run_stat) { 0, 0, 0 }

/*           Link list that acts as a stack to store history commands         */
/*                 Implemented using single linked list                       */

#define MAX_N_HISTORIES 10

typedef struct history_node {
  // shows the index of the most recent histories
  int index;

  int argc;
  char **argv;

  struct history_node *next;

} history_node;

typedef struct history_head {
  // the maximum value is 10
  int n_nodes;

  history_node *first;

} history_head;

#endif
