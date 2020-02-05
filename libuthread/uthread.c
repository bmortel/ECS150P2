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

static uthread_t TIDCount = 0;
static queue_t readyQueue;
static queue_t zombieQueue;
static queue_t blockedQueue;
static struct Tcb* currTcb;
static bool init = false;

//queue_func_t check_tid(void * tcb, uthread_t tid2);

void uthread_yield(void)
{
    void* tcb = malloc(sizeof(struct Tcb));
    struct Tcb* prev = currTcb;

    // Add current thread to queue so it can resume later
    queue_enqueue(readyQueue, currTcb);

    // Deque the oldest thread in the ready queue and switch contexts
    if (queue_dequeue(readyQueue, &tcb) != -1) {
        struct Tcb* next = (struct Tcb*)tcb;
        currTcb = next;
        currTcb->curState = running;
        uthread_ctx_switch(&(prev->ctx), &(currTcb->ctx));
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

    // Change the state of the newly created thread to ready
    tb->curState = ready;
    queue_enqueue(readyQueue, tb);

    return TIDCount;
}

void uthread_exit(int retval)
{
    // Change current thread's state into zombie
    currTcb->curState = zombie;
    queue_enqueue(zombieQueue, currTcb);

    void* tcb = malloc(sizeof(struct Tcb));
    struct Tcb* prev = currTcb;

    // Deque the oldest thread in the ready queue and switch contexts to run next thread
    if (queue_dequeue(readyQueue, &tcb) != -1) {
        currTcb = (struct Tcb *) tcb;
        currTcb->curState = running;
        uthread_ctx_switch(&(prev->ctx), &(currTcb->ctx));

    }

}

int uthread_join(uthread_t tid, int *retval)
{

    void* tcb = malloc(sizeof(struct Tcb));
    struct Tcb* prev = currTcb;

    // Yield until there are no more threads ready to run
    while(1) {

        if (queue_length(readyQueue) == 0) {
            break;
        }
        else{

            // Dequeue the oldest thread in queue then switch to it
            if (queue_dequeue(readyQueue, &tcb) != -1) {
                currTcb = (struct Tcb *) tcb;
                currTcb->curState = running;
                uthread_ctx_switch(&(prev->ctx), &(currTcb->ctx));
            }
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

