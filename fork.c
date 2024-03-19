#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  for (int i = 0; i < 2; i++) {
    pid_t pid = fork();
    if (pid < 0) {
      return -1;
    }
    if (pid == 0) {
      printf("No.%d, Child, Parent pid: %d, Self pid: %d, pid Variable: %d\n", i, getppid(), getpid(), pid);
    } else {
      wait(NULL);
      printf("No.%d, Parent, Parent pid: %d, Self pid: %d, pid Variable: %d\n", i, getppid(), getpid(), pid);
    }
  }
  return 0;
}