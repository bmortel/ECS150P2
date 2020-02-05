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

sigset_t set;

void signalHandler(int signum) { uthread_yield(); }

void preempt_disable(void) {
  /* TODO Phase 4 */
  if (sigdelset(&set, SIGVTALRM)) {
    perror("sigdelset error");
    exit(EXIT_FAILURE);
  }
}

void preempt_enable(void) {
  /* TODO Phase 4 */
  if (sigaddset(&set, SIGVTALRM)) {
    perror("sigaddset error");
    exit(EXIT_FAILURE);
  }
}

void preempt_start(void) {
  /* TODO Phase 4 */
  struct sigaction sigStruct;
  struct itimerval timer;
  sigemptyset(&sigStruct.sa_mask);
  if (sigaddset(&set, SIGVTALRM)) {
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
