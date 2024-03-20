#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 80 /* The maximum length command */
#define MAX_INPUT_SIZE 1024

int tokenize(char *dest[], const size_t size, char *user_input) {
  const char *delimit = " \t\r\n\v\f"; // POSIX whitespace characters
  char *token = strtok(user_input, delimit);

  int count = 0; // representing index of dest strings
  while (token && count < size - 1) {
    // Space must be reallocated before strcpy()
    dest[count] =
        (char *)realloc(dest[count], sizeof(char) * (strlen(token) + 4));
    // Notice that we MUST use strcpy() because the strtok and user_input has it
    // own scope. When user_scope is deleted, the result will be undefined
    strcpy(dest[count], token);
    count++;
    token = strtok(NULL, delimit);
  }
  // set the last string to NULL
  dest[count] = NULL;

  // returns the number of tokens
  return count;
}

int is_empty(const char *str) {
  while (*str != '\0') {
    if (!isspace((unsigned char)*str)) {
      return 0;
    }
    str++;
  }
  return 1;
}

// put tokens in the pointer array and return the number of tokens
// if returns 0, this means there is no input or input only contains with spaces
int get_tokens(char *argv[]) {
  char *user_input = (char *)calloc(sizeof(char), MAX_INPUT_SIZE);

  fgets(user_input, MAX_INPUT_SIZE, stdin);

  // check if the input empty, otherwise the tokenization won't function
  // normally
  if (is_empty(user_input)) {
    free(user_input);
    return 0;
  }

  // tokenize raw string into args, free input, then return the number of tokens
  const int n_tokens = tokenize(argv, MAX_LINE / 2 + 1, user_input);
  free(user_input);

  return n_tokens;
}