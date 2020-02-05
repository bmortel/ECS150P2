#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdbool.h>
#include <limits.h>

#include "context.h"
//#include "preempt.h"
#include "queue.h"
#include "uthread.h"
#include "ThreadControlBlock.h"

uthread_t TIDCount = 0;
queue_t readyQueue;
queue_t zombieQueue;
queue_t blockedQueue;
struct Tcb* currTcb;
bool init = false;

//queue_func_t check_tid(void * tcb, uthread_t tid2);

void uthread_yield(void)
{
    void* tcb = malloc(sizeof(struct Tcb));
    uthread_ctx_t prev = currTcb->ctx;

    // If current state is not blocked, add it to ready queue
    if (currTcb->curState != blocked) {
        currTcb->curState = ready;
        queue_enqueue(readyQueue, currTcb);
    }

    // Deque the oldest thread in the ready queue and switch contexts
	if (queue_dequeue(readyQueue, &tcb) != -1) {

        uthread_ctx_switch(&prev, (&((struct Tcb *) tcb)->ctx));
        currTcb = (struct Tcb *) tcb;
        currTcb->curState = running;
    }
}

uthread_t uthread_self(void)
{
	return currTcb->tid;
}

int uthread_create(uthread_func_t func, void *arg)
{

    // Check if library has been initialized
    if (!init) {
        uthread_init(func, arg);
    }

    struct Tcb* tb = malloc(sizeof(struct Tcb));
    // Allocate memory for stack
    tb->stack = uthread_ctx_alloc_stack();

    // Initialize the context and check if there is an error
    if (uthread_ctx_init(&tb->ctx, tb->stack, func, arg) == -1) {
        return -1;
    }

    // Increment the tid and assign the tid to the newly created thread
    TIDCount++;
    tb->tid = TIDCount;
    tb->curState = ready;
    queue_enqueue(readyQueue, tb);
    return TIDCount;
}

void uthread_exit(int retval)
{
    currTcb->curState = zombie;
    queue_enqueue(zombieQueue, currTcb);

}

int uthread_join(uthread_t tid, int *retval)
{
    void* foundTcb = NULL;
    void* tcb = NULL;

    // Change the current joining thread to block threads
    queue_enqueue(blockedQueue, currTcb);
    currTcb->curState = blocked;

    // Yield until there are no more threads ready to run
    while(1) {

        if (queue_length(readyQueue) == 0) {
            break;
        }
        else{
            uthread_yield();
        }
    }



    return 1;

}
void uthread_init(uthread_func_t func, void *arg) {

    // Assign queues for different state
    readyQueue = queue_create();
    zombieQueue = queue_create();
    blockedQueue = queue_create();

    // Create a thread for main
    struct Tcb* main = (struct Tcb*)malloc(sizeof(struct Tcb));
    main->tid = 0;
    main->stack = uthread_ctx_alloc_stack();

    // Set the current thread to main
    currTcb = main;
    init = true;
}

/*
queue_func_t check_tid(void * tcb, uthread_t tid2) {
    if (((struct Tcb*)tcb)->tid == tid2) {
        return (queue_func_t) 1;
    }
}
*/

