#include "banker.h"
#include <stdbool.h>
#include <stdio.h>

bool is_in_safe_state(void) {
  // Step 1:
  // Work = Available
  int work[NUMBER_OF_RESOURCES];
  for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
    work[i] = available[i];
  }

  // Finish[i] = false
  int finish[NUMBER_OF_CUSTOMERS];
  for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
    finish[i] = false;
  }

  // Step 2:
  // Find an index i such that both
  //   a. Finish[i] == false
  //   b. Need i <= Work
  // If no such i exists, go to step 4.
  int index = -1;
  while ((index = find_proper_index(work, finish)) != -1) {
    // Step 3:
    // Work = Work + Allocation_i
    for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
      work[j] += allocation[index][j];
    }
    // Finish[i] = true
    finish[index] = true;
    // Go to step 2
  }
  // Step 4:
  // If Finish[i] == true for all i, then the system is in a safe state.
  for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
    // else the system is not safe
    if (finish[i] != true) {
      return false;
    }
  }
  // the system is safe
  return true;
}

int find_proper_index(const int work[], const int finish[]) {
  for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
    // a. Finish[i] == false
    if (finish[i] != false) {
      continue;
    }
    // b. Need_i <= Work
    bool flag = true;
    for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
      if (need[i][j] > work[j]) {
        flag = false;
        break;
      }
    }
    if (flag == false) {
      continue;
    }
    // Both satisfied, return index
    return i;
  }
  // index not found, return -1
  return -1;
}