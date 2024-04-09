# Using POSIX Multithread to estimate $\pi$

## Running

### Prerequisite

Your Operating System must be POSIX compatible. This program will not run on Windows.

### Compile and Run

```bash
clang src/main.c -o main
./main
```

The expected result is something like

```bash
The estimated value of pi is: 3.123047
```

### Run with Time Counter

You can also find the running time using 

```bash
time ./main
```

The expected result may be something like

```bash
The estimated value of pi is: 3.142395 
./main  0.22s user 1.03s system 99% cpu 1.249 total
```

### Run Multiple Times to Test Stability

```bash
for i in {0..100} 
./main
```

The output is

```bash
The estimated value of pi is: 3.149900 
The estimated value of pi is: 3.151700 
The estimated value of pi is: 3.149900 
The estimated value of pi is: 3.149950 
The estimated value of pi is: 3.149950 
...
The estimated value of pi is: 3.149900 
The estimated value of pi is: 3.151700 
The estimated value of pi is: 3.149900 
The estimated value of pi is: 3.151650 
The estimated value of pi is: 3.152200 
...
The estimated value of pi is: 3.149950 
The estimated value of pi is: 3.151750 
The estimated value of pi is: 3.149900 
The estimated value of pi is: 3.151700 
The estimated value of pi is: 3.149900 
```

The deferences between different results are small.

They are all good approximation of $\pi$ (3.14159) .

## Features

This program invokes multithread and semaphores to accelerate the calculation of $\pi$.

### Multithread

This program invokes `pthread` to achieve multithread. 

The program first creates two arrays containing `pthread_t` and `pthread_attr_t ` (the length of the array is defined as `NUM_THREAD`), then init and create thread using the two arrays. Each thread will execute the code in the `runner` function, updating the the global variables `n_points` and `n_points_in_circle`. After execution, the thread will exit. The main thread will wait for every thread to quit using `pthread_join` .

### Semaphore

When different threads want to update the the global variables, race condition will occur. To prevent this, the program invokes semaphore.

The semaphore `sem_counter` is initalized as a pointer as global variable in order to share with the child threads. When the program begins, the sem_counter is initialized as a named semaphore(the name is defined as `SEM_COUNTER_NAME`).

When the child threads executing `runner`, they will add 1 to the counters `n_points` and `n_points_in_circle`. Race condition may occur without semaphore. The semaphore allows only one thread to manipulate the counters. The threads uses `sem_wait` to acquire semaphore and `sem_post` to release semaphore.

The semaphore will be closed and unlinked when the program ends, this is achieved using `sem_close` and `sem_unlink`.

## References

- [POSIX Semaphores in Linux - SoftPrayog](https://www.softprayog.in/programming/posix-semaphores)

- [pthread_join (opengroup.org)](https://pubs.opengroup.org/onlinepubs/009695399/functions/pthread_join.html)

- [Random double generator problem (C Programming) (ubuntuforums.org)](https://ubuntuforums.org/showthread.php?t=1717717&p=10618266#post10618266)

