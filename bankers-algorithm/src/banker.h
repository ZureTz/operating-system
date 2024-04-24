#pragma once
#ifndef BANKER_H
#define BANKER_H

#include <stdbool.h>

/* these may be any values >= 0 */
#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 3

/* the available amount of each resource */
extern int available[NUMBER_OF_RESOURCES];

/* the maximum demand of each customer */
extern int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/* the amount currently allocated to each customer */
extern int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/* the remaining need of each customer */
extern int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

// check if the system is in safe state
bool is_in_safe_state(void);

// Find proper index(used in is_in_safe_state())
int find_proper_index(const int work[], const int finish[]);

#endif