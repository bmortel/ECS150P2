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
#include "preempt.h"
#include "queue.h"
#include "uthread.h"
#include "ThreadControlBlock.h"

static uthread_t TIDCount = 0;
static queue_t readyQueue;
static queue_t zombieQueue;
static queue_t blockedQueue;
static struct Tcb* currTcb;
static bool init = false;

bool check_tid(void * tcb, uthread_t tid2);

void uthread_yield(void)
{
    //preempt_disable();
    void* tcb = malloc(sizeof(struct Tcb));
    struct Tcb* prev = currTcb;

    // Add current thread to queue so it can resume later
    if (currTcb->curState != blocked) {
        queue_enqueue(readyQueue, currTcb);
    }

    // Deque the oldest thread in the ready queue and switch contexts
    if (queue_dequeue(readyQueue, &tcb) != -1) {
        struct Tcb* next = (struct Tcb*)tcb;
        currTcb = next;
        currTcb->curState = running;
        uthread_ctx_switch(&(prev->ctx), &(currTcb->ctx));
    }
    //preempt_enable();
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
    //preempt_disable();
    TIDCount++;
    tb->tid = TIDCount;

    // Change the state of the newly created thread to ready
    tb->curState = ready;
    queue_enqueue(readyQueue, tb);
    //preempt_enable();
    return TIDCount;
}

void uthread_exit(int retval)
{   
    //preempt_disable();
    // Change current thread's state into zombie
    currTcb->curState = zombie;
    queue_enqueue(zombieQueue, currTcb);
    currTcb->retval = retval;

    void* tcb = malloc(sizeof(struct Tcb));
    struct Tcb* prev = currTcb;

    // Deque the oldest thread in the ready queue and switch contexts to run next thread
    if (queue_dequeue(readyQueue, &tcb) != -1) {
        currTcb = (struct Tcb *) tcb;
        currTcb->curState = running;
        uthread_ctx_switch(&(prev->ctx), &(currTcb->ctx));

    }
    //preempt_enable();

}

int uthread_join(uthread_t tid, int *retval)
{
    if (tid == currTcb->tid || tid == 0) {
        return -1;
    }
    void* tcb = malloc(sizeof(struct Tcb));
    struct Tcb* prev = currTcb;

    currTcb->curState = blocked;
    queue_enqueue(blockedQueue, currTcb);

    // Look through ready queue
    queue_iterate(readyQueue, (queue_func_t) check_tid, (void *)&tid, tcb);
    if (tcb == NULL) {
        return -1;
    }
    struct Tcb* joining = (struct Tcb*) tcb;


    // Look through zombie queue
    /*tcb = malloc(sizeof(struct Tcb));
    queue_iterate(zombieQueue, (queue_func_t) check_tid, (void *)&tid, tcb);
    if (tcb == NULL) {
        return -1;
    }*/
    // If child is not found then it doesn't exist or is blocked

    // Yield until there are no more threads ready to run
    while(joining->curState != zombie) {

        uthread_yield();
    }
    *retval = joining->retval;
    queue_delete(zombieQueue, joining);
    queue_delete(blockedQueue, prev);
    currTcb = prev;
    free(joining);
    uthread_yield();

    return 1;

}
void uthread_init() {

    // Assign queues for different state
    readyQueue = queue_create();
    zombieQueue = queue_create();
    blockedQueue = queue_create();

    // Create a thread for main
    struct Tcb* mainT = (struct Tcb*)malloc(sizeof(struct Tcb));
    mainT->tid = 0;
    mainT->stack = uthread_ctx_alloc_stack();

    // Set the current thread to main
    currTcb = mainT;
    init = true;
    //preempt_start();
}


bool check_tid(void * tcb, uthread_t tid2) {
    if (((struct Tcb*)tcb)->tid == tid2) {
        return true;
    }
    return false;
}




