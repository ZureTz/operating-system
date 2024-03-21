#ifndef SHELL_PREPROCESS_H
#define SHELL_PREPROCESS_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "history.h"
#include "types.h"

/*                Built-in commands                */

run_stat shell_history_commands(int argc, char *argv[],
                                history_head *head) {
  shell_history_impl(argc, argv, head);
  return RUN_BUILTIN_COMMAND;
}

run_stat shell_cd(const char *path) {
  int stat = chdir(path);

  // invalid path
  if (stat < 0) {
    perror(path);
  }

  return RUN_BUILTIN_COMMAND;
}

run_stat shell_pwd() {
  char *cwd = (char *)malloc(sizeof(char) * MAX_BUFFER_SIZE);

  // get current working dir, and print
  getcwd(cwd, MAX_BUFFER_SIZE);
  printf("%s\n", cwd);

  free(cwd);

  return RUN_BUILTIN_COMMAND;
}

run_stat shell_exit() { return EXIT_STAT; }

run_stat preprocess(int argc, char *argv[], history_head *head,
                    int *const command_index) {
  // history: show the most recent 10 commands
  if (is_history_command(argv[0])) {
    (*command_index)--;
    return shell_history_commands(argc, argv, head);
  }
  // record the history
  record_recent_history(head, argc, argv, *command_index);

  // built-in command list
  // 1. cd: change current woring directory
  if (strcmp(argv[0], "cd") == 0) {
    return shell_cd(argv[1]);
  }
  // 2. pwd: print current working directory
  if (strcmp(argv[0], "pwd") == 0) {
    return shell_pwd();
  }
  // 3. exit: exit the shell
  if (strcmp(argv[0], "exit") == 0) {
    return shell_exit();
  }

  // check if the input token contains "&"
  // if last_token == "&",  no need to wait
  if (strcmp(argv[argc - 1], "&") == 0) {
    // also, this symbol should not be passed as a argument to exec()
    // removing the last argument
    argv[argc - 1] = NULL;
    return FORK_AND_NO_WAIT;
  }

  // default return status
  return FORK_AND_WAIT;
}

#endif