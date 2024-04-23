/*
  Using multithread to estimate π (Monte Carlo technique)
*/

#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Number of threads
#define NUM_THREADS 8
// Batch size (how many point to generate) for each thread running
#define BATCH_SIZE 25000
// Name of semaphore
#define SEM_COUNTER_NAME "/sem-counter"

sem_t *sem_counter;
unsigned n_points = 0;
unsigned n_points_in_circle = 0;

struct Point {
  double x;
  double y;
};

/* generate a random floating point number from min to max */
double randfrom(double min, double max) {
  double range = (max - min);
  double div = RAND_MAX / range;
  return min + (rand() / div);
}

// find out if the point is in the circle
bool is_in_circle(const struct Point point) {
  // x^2 + y^2 <= 1
  return ((point.x * point.x) + (point.y * point.y)) <= 1.0;
}

// Calculation mathod in other threads
void *runner(void *param) {
  for (int i = 0; i < BATCH_SIZE; i++) {
    struct Point point;
    point.x = randfrom(-1, 1);
    point.y = randfrom(-1, 1);

    // acquire semaphore
    sem_wait(sem_counter);

    n_points++;
    // if in circle, increments n_points_in_circle
    if (is_in_circle(point)) {
      n_points_in_circle++;
    }

    // release semaphore
    sem_post(sem_counter);
  }
  pthread_exit(0);
}

int main(void) {
  // semaphore initialization
  // only one thread can use counter to prevent race condition
  if ((sem_counter = sem_open(SEM_COUNTER_NAME, O_CREAT, 0660, 1)) ==
      SEM_FAILED) {
    perror("sem_open");
    exit(1);
  }

  // Create several threads, each of which generates random points and
  // determines if the points fall within the circle.
  // Array of threads to be created
  pthread_t worker_tids[NUM_THREADS];
  pthread_attr_t worker_attrs[NUM_THREADS];

  for (int i = 0; i < NUM_THREADS; i++) {
    // get default init attributes
    pthread_attr_init(&worker_attrs[i]);
    // create thread
    pthread_create(&worker_tids[i], &worker_attrs[i], runner, NULL);
  }

  // Wait for each thread to exit
  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(worker_tids[i], NULL);
  }

  // calculate pi
  const double pi = 4.0 * (double)n_points_in_circle / (double)n_points;

  printf("The estimated value of pi is: %lf \n", pi);

  // close and unlink semaphore
  sem_close(sem_counter);
  sem_unlink(SEM_COUNTER_NAME);

  return 0;
}