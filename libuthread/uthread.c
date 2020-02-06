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
static struct Tcb* currTcb;
static bool init = false;

bool check_tid(void * tcb, uthread_t* tid2);

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
        uthread_init();
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

    tb->joined = NULL;

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

    // If the thread is joining another thread,
    // Change the parent thread's state to ready and add it to ready queue
    if (currTcb->joined != NULL) {
        currTcb = currTcb->joined;
        currTcb->curState = ready;
        queue_enqueue(readyQueue, currTcb);
    }

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

    // Change the parent thread's state to blocked
    currTcb->curState = blocked;

    // Look through ready queue
    queue_iterate(readyQueue, (queue_func_t) check_tid, (void *)&tid, &tcb);
    if (tcb == NULL) {
        // If thread not found in ready queue, look through zombie queue
        queue_iterate(zombieQueue, (queue_func_t) check_tid, (void *)&tid, &tcb);

        if (tcb == NULL) {
            return -1;
        }
    }

    struct Tcb* joining = (struct Tcb*) tcb;

    // Delete child thread from zombie queue if it is in zombie queue
    queue_delete(zombieQueue, joining);
    joining->joined = currTcb;


    // Yield until child thread is ran
    while(joining->curState != zombie) {
        uthread_yield();
    }

    // Assign return value of child
    *retval = joining->retval;

    return 0;

}
void uthread_init() {

    // Assign queues for different state
    readyQueue = queue_create();
    zombieQueue = queue_create();

    // Create a thread for main
    struct Tcb* mainT = (struct Tcb*)malloc(sizeof(struct Tcb));
    mainT->tid = 0;
    mainT->stack = uthread_ctx_alloc_stack();

    // Set the current thread to main
    currTcb = mainT;
    init = true;
    //preempt_start();
}


bool check_tid(void * tcb, uthread_t* tid2) {
    if (((struct Tcb*)tcb)->tid == *tid2) {
        return true;
    }
    return false;
}




