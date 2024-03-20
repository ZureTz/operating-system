#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int fork_new_process(char *const argv[], int wait_flag) {
  // After reading user input, the steps are:
  // (1) fork a child process using fork()
  int child_stat_loc;
  pid_t pid = fork();

  if (pid < 0) {
    perror("Fork faild");
    exit(1);
  }

  if (pid == 0) {
    // (2) the child process will invoke execvp()
    int exec_stat = execvp(argv[0], argv);

    // return -1 if the execution has failed
    if (exec_stat < 0) {
      perror(argv[0]);
      exit(1);
    }

    // return 0;
    printf("Shouldnt be printed, %d\n", exec_stat);
  }

  // if pid > 0, the parent process continues

  // (3) if command included &, parent will not invoke wait()
  if (wait_flag == 0) {
    return 0;
  }

  // wait for child process to finish
  waitpid(pid, &child_stat_loc, WUNTRACED);
  return 0;
}