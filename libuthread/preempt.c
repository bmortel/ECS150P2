#include "preempt.h"

#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

struct sigaction sigActionStruct;
struct itimerval timer;
struct sigaction oldAction;

// Force running thread to yield
void signalHandler() { uthread_yield(); }


void preempt_disable(void) {

  // Restore default SIGVTALRM action
  if (sigaction(SIGVTALRM, &oldAction, NULL) == -1) {
    perror("sigaction error");
    exit(EXIT_FAILURE);
  }
}

void preempt_enable(void) {

  // Restore our SIGVTALRM handler
  if (sigaction(SIGVTALRM, &sigActionStruct, NULL) == -1) {
    perror("sigaction error");
    exit(EXIT_FAILURE);
  }
}

void preempt_start(void) {

	// Set mask to empty set 
  sigemptyset(&sigActionStruct.sa_mask);

	// Install signal handler
  sigActionStruct.sa_flags = 0;
  sigActionStruct.sa_handler = signalHandler;
  if (sigaction(SIGVTALRM, &sigActionStruct, &oldAction) == -1) {
    perror("sigaction error");
    exit(EXIT_FAILURE);
  }

  // Set alarm initial tick and interval
  timer.it_value.tv_sec = 1 / HZ;
  timer.it_interval = timer.it_value;

  // Initialize timer
  if (setitimer(ITIMER_VIRTUAL, &timer, NULL) == -1) {
    perror("settimer error");
    exit(EXIT_FAILURE);
  }
}
