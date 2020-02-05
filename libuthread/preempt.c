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

struct sigaction sigStruct;
struct itimerval timer;


void signalHandler(int signum) { uthread_yield(); }

void preempt_disable(void) {
  // Remove SIGVTALRM from mask
  if (sigdelset(&sigStruct.sa_mask, SIGVTALRM)) {
    perror("sigdelset error");
    exit(EXIT_FAILURE);
  }
}

void preempt_enable(void) {
  // Add SIGVTALRM back to mask
  if (sigaddset(&sigStruct.sa_mask, SIGVTALRM)) {
    perror("sigaddset error");
    exit(EXIT_FAILURE);
  }
}

void preempt_start(void) {
  sigemptyset(&sigStruct.sa_mask);
  if (sigaddset(&sigStruct.sa_mask, SIGVTALRM)) {
    perror("sigaddset error");
    exit(EXIT_FAILURE);
  }
  sigStruct.sa_flags = 0;
  sigStruct.sa_handler = signalHandler;
  if (sigaction(SIGVTALRM, &sigStruct, NULL) == -1) {
    perror("sigaction error");
    exit(EXIT_FAILURE);
  }
  timer.it_value.tv_sec = 1 / HZ;
  timer.it_interval = timer.it_value;
  if (setitimer(ITIMER_VIRTUAL, &timer, NULL) == -1) {
    perror("settimer error");
    exit(EXIT_FAILURE);
  }
}
