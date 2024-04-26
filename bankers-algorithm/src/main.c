#include "banker.h"
#include "customer.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// the available amount of each resource
int available[NUMBER_OF_RESOURCES];
// the maximum demand of each customer
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
// the amount currently allocated to each customer
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
// the remaining need of each customer
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

// mutex lock to prevent race conditons
pthread_mutex_t mutex;

// customer activty to request and release resources
void *customer_activity(void *args);

int main(int argc, char const *argv[]) {
  // Check if the argc is correct
  if (argc != NUMBER_OF_RESOURCES + 1) {
    fprintf(
        stderr,
        "Usage: ./main [# of resource 1] [# of resource 2] [# of resource 3]");
    return -1;
  }

  // Initialize mutex lock
  if (pthread_mutex_init(&mutex, NULL) == -1) {
    perror("mutex_init error");
    return -1;
  }

  // Initialize the arrays

  // 1. Initialize `available` via given arguments
  for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
    const int num_resource = atoi(argv[i + 1]);
    if (num_resource < 0) {
      fprintf(stderr, "The given number of resources must be non-negative.");
      return -1;
    }
    available[i] = num_resource;
  }

  // 2.
  // Initialize `maximum` using random number generator
  // Also initialize `allocation` to all 0 (not allocated yet)
  // Also note that `need` is the same as maximum currently since `allocation`
  // is all 0

  srand(time(NULL)); // set random number seed
  for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
    for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
      // Range: from 0 to current available resources
      need[i][j] = maximum[i][j] = rand() % (available[j] + 1);
      allocation[i][j] = 0;
    }
  }

  // 3. Create threads
  pthread_t customer_tids[NUMBER_OF_CUSTOMERS];
  pthread_attr_t customer_attrs[NUMBER_OF_CUSTOMERS];
  int customer_nums[NUMBER_OF_CUSTOMERS];

  for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
    // store number in array to prevent confilcts
    customer_nums[i] = i;

    // get default init attributes
    pthread_attr_init(&customer_attrs[i]);

    // create thread
    pthread_create(&customer_tids[i], &customer_attrs[i], customer_activity,
                   &customer_nums[i]);
  }
  for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
    // Wait for each thread to exit
    pthread_join(customer_tids[i], NULL);
  }

  printf("\nThe program successfully finished.\n");
  pthread_mutex_destroy(&mutex);
  return 0;
}

void *customer_activity(void *param) {
  const int customer_num = *(int *)param;

  for (int i = 0; i < 3; i++) {
    // Generate request using rand()
    int request[NUMBER_OF_RESOURCES];
    int release[NUMBER_OF_RESOURCES];

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
      release[i] = request[i] = rand() % (need[customer_num][i] + 1);
    }

    // wait for response
    while (request_resources(customer_num, request) == -1) {
      // If not successful, wait for 1s.
      sleep(1);
      continue;
    }
    // Do something
    // ...

    sleep(1);

    // ...

    release_resources(customer_num, release);
  }

  pthread_exit(0);
}