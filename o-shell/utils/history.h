#ifndef SHELL_HISTORY_H
#define SHELL_HISTORY_H

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"

/*             Data Structure Utilities            */

void free_history_node(history_node *node) {
  if (node == NULL) {
    return;
  }

  // free the string tokens
  char **ptr = node->argv;
  while (ptr && *ptr != NULL) {
    free(*ptr);
    ptr++;
  }
  free(node->argv);

  free(node);
}

void history_queue_pop_back(history_head *head) {
  history_node *node = head->first;
  while (node && node->next && node->next->next) {
    node = node->next;
  }

  if (node && node->next) {
    free_history_node(node->next);
    node->next = NULL;
  }
}

void history_queue_push_front(history_head *head, history_node *node) {
  node->next = head->first;
  head->first = node;

  if (head->n_nodes < MAX_N_HISTORIES) {
    head->n_nodes++;
    return;
  }
  // if there are max histories
  history_queue_pop_back(head);
}

void record_recent_history(history_head *head, char *argv[],
                           int command_index) {
  // initialize
  history_node *node = (history_node *)malloc(sizeof(history_node));
  node->index = command_index;
  node->argv = argv;
  node->next = NULL;

  // insert to the queue
  history_queue_push_front(head, node);
}

void free_all_histories(history_head *head) {
  history_node *node = head->first;
  while (node) {
    history_node *temp = node;
    node = node->next;
    free_history_node(temp);
  }
}

/*              Command processing              */

// Check if the command contains history commands
// note: the program will not record command containing
// history commands
int is_history_command(char *argv[]) {
  if (strcmp(argv[0], "history") == 0) {
    return 1;
  }
  if (strcmp(argv[0], "!!") == 0) {
    return 1;
  }
  if (argv[0][0] == '!' && isdigit(argv[0][1])) {
    return 1;
  }
  return 0;
}

run_stat shell_history_impl(const char *command, history_head *head) {
  if (head == NULL) {
    perror("Missing reference to history records.");
    return EXIT_STAT;
  }

  if (strcmp(command, "history") == 0) {
    history_node *ptr = head->first;

    if (ptr == NULL) {
      printf("No history command yet\n");
      return RUN_BUILTIN_COMMAND;
    }

    while (ptr) {
      printf("\t%d\t", ptr->index);
      char **command_ptr = ptr->argv;
      while (command_ptr && *command_ptr) {
        printf("%s ", *command_ptr);
        command_ptr++;
      }
      printf("\n");
      ptr = ptr->next;
    }
    return RUN_BUILTIN_COMMAND;
  }

  if (strcmp(command, "!!") == 0) {
    // replace current argv with previous command
    // then continue the fork process
    
  }
}

#endif