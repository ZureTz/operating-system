# Banker’s Algorithm

This project is a multithreaded program that implements the banker’s algorithm.

Several customers request and release resources from the bank. The banker will grant a request only if it leaves the system in a safe state.

This program combines three separate topics: 

- multithreading
- preventing race conditions
- deadlock avoidance

## Build and Run

### Prerequisites 

Your system must be posix-compatible since this program invokes `pthread` library.

### Build

Open the terminal, change path to the project directory, then run:

```bash
make
```

### Run

After `make`, an executable file `main` will appear in the folder.

You can run the program as shown below:

```bash
./main  [# of resource 1] [# of resource 2] [# of resource 3] 
```

## Features

### Multithreading

- The code creates multiple threads (customers) using `pthread` library functions.
- Each customer thread simulates independent activity requesting and releasing resources.
- The `customer_activity` function simulates customer activity:
    - It generates random requests and release amounts for resources within the customer's current need.
    - It calls a`request_resources` function that likely implements the Banker's Algorithm to check for safe sequences before granting requests.
    - The customer sleeps for a while to simulate some work before releasing resources.

### Race condition prevention

A mutex lock (`mutex`) is used to prevent race conditions when multiple threads (customers) access shared resources.

### Banker's Algorithm Implementation

- The code defines three resources (`NUMBER_OF_RESOURCES`) and allows specifying the initial available amount for each through command-line arguments (before the compilation process).
- It keeps track of three key data structures:
    - `available`: Stores the current available amount of each resource.
    - `maximum`: Stores the maximum demand of each customer (process) for each resource.
    - `allocation`: Stores the amount of each resource currently allocated to each customer.
- The `need` matrix is initially the same as `maximum` because no resources are allocated yet. It gets updated as resources are requested and released.

## Program output and explanation

On running program like below:

```bash
./main 256 128 223
```

Waiting for a few seconds, the program will exit normally with output similar to below:

```bash
Customer 0 is requesting for resources:
7 55 107 
Current available Resources: 
256 128 223 
The need: 
145 61 110 
Request is suceessfully granted.

Customer 1 is requesting for resources:
111 40 97 
Current available Resources: 
249 73 116 
The need: 
243 71 152 
Request is suceessfully granted.

Customer 2 is requesting for resources:
101 0 10 
Current available Resources: 
138 33 19 
The need: 
109 3 56 
Request not suceessful: the request will make unsafe conditions.

Customer 3 is requesting for resources:
118 36 41 
Current available Resources: 
138 33 19 
The need: 
242 99 59 
Request not suceessful: maximum available resources exceeded.

Customer 4 is requesting for resources:
56 4 9 
Current available Resources: 
138 33 19 
The need: 
190 10 175 
Request not suceessful: the request will make unsafe conditions.

Customer 0 is requesting for resources:
23 55 93 
Current available Resources: 
145 88 126 
The need: 
145 61 110 
Request is suceessfully granted.

Customer 3 is requesting for resources:
118 36 41 
Current available Resources: 
233 73 130 
The need: 
242 99 59 
Request not suceessful: the request will make unsafe conditions.

Customer 1 is requesting for resources:
167 24 44 
Current available Resources: 
233 73 130 
The need: 
243 71 152 
Request not suceessful: the request will make unsafe conditions.

Customer 4 is requesting for resources:
56 4 9 
Current available Resources: 
233 73 130 
The need: 
190 10 175 
Request is suceessfully granted.

Customer 2 is requesting for resources:
101 0 10 
Current available Resources: 
177 69 121 
The need: 
109 3 56 
Request is suceessfully granted.

Customer 1 is requesting for resources:
167 24 44 
Current available Resources: 
132 73 120 
The need: 
243 71 152 
Request not suceessful: maximum available resources exceeded.

Customer 4 is requesting for resources:
190 2 97 
Current available Resources: 
132 73 120 
The need: 
190 10 175 
Request not suceessful: maximum available resources exceeded.

Customer 2 is requesting for resources:
23 0 13 
Current available Resources: 
233 73 130 
The need: 
109 3 56 
Request is suceessfully granted.

Customer 0 is requesting for resources:
2 18 73 
Current available Resources: 
233 128 210 
The need: 
145 61 110 
Request is suceessfully granted.

Customer 3 is requesting for resources:
118 36 41 
Current available Resources: 
231 110 137 
The need: 
242 99 59 
Request is suceessfully granted.

Customer 2 is requesting for resources:
95 0 9 
Current available Resources: 
136 74 109 
The need: 
109 3 56 
Request is suceessfully granted.

Customer 1 is requesting for resources:
167 24 44 
Current available Resources: 
41 74 100 
The need: 
243 71 152 
Request not suceessful: maximum available resources exceeded.

Customer 4 is requesting for resources:
190 2 97 
Current available Resources: 
41 74 100 
The need: 
190 10 175 
Request not suceessful: maximum available resources exceeded.

Customer 3 is requesting for resources:
175 49 6 
Current available Resources: 
161 128 214 
The need: 
242 99 59 
Request not suceessful: maximum available resources exceeded.

Customer 1 is requesting for resources:
167 24 44 
Current available Resources: 
256 128 223 
The need: 
243 71 152 
Request is suceessfully granted.

Customer 4 is requesting for resources:
190 2 97 
Current available Resources: 
89 104 179 
The need: 
190 10 175 
Request not suceessful: maximum available resources exceeded.

Customer 3 is requesting for resources:
175 49 6 
Current available Resources: 
89 104 179 
The need: 
242 99 59 
Request not suceessful: maximum available resources exceeded.

Customer 1 is requesting for resources:
138 18 27 
Current available Resources: 
256 128 223 
The need: 
243 71 152 
Request is suceessfully granted.

Customer 4 is requesting for resources:
190 2 97 
Current available Resources: 
118 110 196 
The need: 
190 10 175 
Request not suceessful: maximum available resources exceeded.

Customer 3 is requesting for resources:
175 49 6 
Current available Resources: 
118 110 196 
The need: 
242 99 59 
Request not suceessful: maximum available resources exceeded.

Customer 4 is requesting for resources:
190 2 97 
Current available Resources: 
256 128 223 
The need: 
190 10 175 
Request is suceessfully granted.

Customer 3 is requesting for resources:
175 49 6 
Current available Resources: 
66 126 126 
The need: 
242 99 59 
Request not suceessful: maximum available resources exceeded.

Customer 4 is requesting for resources:
145 4 131 
Current available Resources: 
256 128 223 
The need: 
190 10 175 
Request is suceessfully granted.

Customer 3 is requesting for resources:
175 49 6 
Current available Resources: 
111 124 92 
The need: 
242 99 59 
Request not suceessful: maximum available resources exceeded.

Customer 3 is requesting for resources:
175 49 6 
Current available Resources: 
256 128 223 
The need: 
242 99 59 
Request is suceessfully granted.

Customer 3 is requesting for resources:
27 3 52 
Current available Resources: 
256 128 223 
The need: 
242 99 59 
Request is suceessfully granted.

The program successfully finished.
```

This shows the simulation of a resource allocation system using the Banker's Algorithm to prevent deadlocks. Here's a breakdown of what happened:

### Initial State

- Available resources: 256, 128, 223 (for 3 unspecified resources)
- Multiple customers (5) with their maximum demands and current needs.

### Customer Requests

- Each customer repeatedly requests resources.
- The code simulates successful requests and unsuccessful requests based on safety checks:
    - Successful requests: When a request doesn't lead to a potential deadlock situation (safe sequence).
    - Unsuccessful requests: When a request could lead to a deadlock (unsafe sequence) or exceeds the maximum available resources.

### Banker's Algorithm in Action

- The `request_resources` function implements the Banker's Algorithm to determine if granting a request is safe.
- It considers the current available resources, the maximum demands of all customers, and the current allocation of resources to ensure a safe sequence of resource allocation exists.

### Test Results

- Customer 0, 2, and 4 had successful requests at various points.
- Customer 1, 3, and 4 faced rejections due to potential deadlocks or exceeding available resources.
- The simulation demonstrates how the Banker's Algorithm prevents deadlocks by ensuring safe resource allocation.

## Reference

- [ahmedwael19/CIE302---Project2---BankerAlgorithm: Implementing Multithreaded Banker Algorithm (github.com)](https://github.com/ahmedwael19/CIE302---Project2---BankerAlgorithm)

- [c++ - How do I use extern to share variables between source files? - Stack Overflow](https://stackoverflow.com/questions/1433204/how-do-i-use-extern-to-share-variables-between-source-files)

- [[Makefile] String Replacement Example (siongui.github.io)](https://siongui.github.io/2016/04/08/makefile-string-replace/)