#include "customer.h"
#include "banker.h"
#include <pthread.h>
#include <stdio.h>

// mutex lock to prevent race conditons
extern pthread_mutex_t mutex;

int request_resources(int customer_num, int request[]) {
  pthread_mutex_lock(&mutex);

  /*******   Logging Segment     *******/

  printf("\nCustomer %d is requesting for resources:\n", customer_num);
  for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
    printf("%d ", request[i]);
  }

  printf("\nCurrent available Resources: \n");
  for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
    printf("%d ", available[i]);
  }

  printf("\nThe need: \n");
  for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
    printf("%d ", need[customer_num][i]);
  }

  /*******     Logging Ends      *******/

  // Step 1:
  for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
    // If Request <= Need_i, go to step 2.
    if (request[i] <= need[customer_num][i]) {
      continue;
    }
    // Otherwise, raise an error condition, since the process has exceeded its
    // maximum claim.
    printf("\nRequest not suceessful: maximum need exceeded.\n");
    pthread_mutex_unlock(&mutex);
    return -1;
  }

  // Step 2:
  for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
    // If Request <= Available, go to step 3.
    if (request[i] <= available[i]) {
      continue;
    }
    // Otherwise, must wait, since the resources are not available.
    printf("\nRequest not suceessful: maximum available resources exceeded.\n");
    pthread_mutex_unlock(&mutex);
    return -1;
  }

  // Step 3:
  // Apply Changes
  for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
    available[i] -= request[i];
    allocation[customer_num][i] += request[i];
    need[customer_num][i] -= request[i];
  }

  // Check if the change is safe
  // if not safe:
  if (!is_in_safe_state()) {
    // Revert changes
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
      available[i] += request[i];
      allocation[customer_num][i] -= request[i];
      need[customer_num][i] += request[i];
    }
    // Release lock and return -1
    printf(
        "\nRequest not suceessful: the request will make unsafe conditions.\n");
    pthread_mutex_unlock(&mutex);
    return -1;
  }

  // if safe:
  printf("\nRequest is suceessfully granted.\n");
  pthread_mutex_unlock(&mutex);
  return 0;
}

int release_resources(int customer_num, int release[]) {
  pthread_mutex_lock(&mutex);

  // Release resources
  for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
    available[i] += release[i];
    allocation[customer_num][i] -= release[i];
    need[customer_num][i] += release[i];
  }

  pthread_mutex_unlock(&mutex);
  return 0;
}
