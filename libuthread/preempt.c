#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "preempt.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

void signalHandler (int signum){
	uthread_yield();
}

void preempt_disable(void)
{
	/* TODO Phase 4 */
}

void preempt_enable(void)
{
	/* TODO Phase 4 */
}

void preempt_start(void)
{
	/* TODO Phase 4 */
	struct sigaction sigStruct;
	struct itimerval timer;
	sigemptyset(&sigStruct.sa_mask);
	sigStruct.sa_flags = 0;
	sigStruct.sa_handler = signalHandler;
	if (sigaction(SIGVTALRM, &sigStruct, NULL) == -1){
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
	timer.it_value.tv_sec = 1/HZ;
	timer.it_interval = timer.it_value;
	if (setitimer(ITIMER_VIRTUAL, &timer, NULL) == -1){
		perror("timer");
		exit(EXIT_FAILURE);
	}

	
}

