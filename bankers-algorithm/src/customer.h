#pragma once
#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "banker.h"

/* These two functions should return 0 if successful (the request has been
 * granted) and –1 if unsuccessful.
 */
int request_resources(int customer_num, int request[]);

int release_resources(int customer_num, int release[]);

#endif