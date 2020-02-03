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
queue_t tcbQueue;
queue_t zombieQueue;
int currTID = 0;
struct Tcb* currTcb;
bool init = 0;


/* TODO Phase 2 */

void uthread_yield(void)
{
    void* tcb;
    uthread_ctx_t prev;
	queue_enqueue(tcbQueue,currTcb);
	if (queue_dequeue(tcbQueue,tcb) != -1) {
        uthread_ctx_switch(&prev, &((struct Tcb *) tcb)->ctx);
        currTcb = (struct Tcb *) tcb;
    }
}

uthread_t uthread_self(void)
{
	return currTcb->tid;
}

int uthread_create(uthread_func_t func, void *arg)
{
    if (TIDCount < USHRT_MAX - 1) {

        if (!init) {
            uthread_init(func, arg);
        }

        struct Tcb *tcb = malloc(sizeof(struct Tcb));
        tcb->stack = uthread_ctx_alloc_stack();
        if (uthread_ctx_init(&tcb->ctx, tcb->stack, func, arg) == -1) {
            return -1;
        }
        TIDCount++;
        tcb->tid = TIDCount;
        tcb->state = "ready";
        queue_enqueue(tcbQueue, tcb);
        return TIDCount;
    }
    else {
        return -1;
    }
}

void uthread_exit(int retval)
{
    currTcb->state = "zombie";
    queue_enqueue(zombieQueue, currTcb);

}

int uthread_join(uthread_t tid, int *retval)
{
    while(1) {
        if (queue_length(tcbQueue) == 0) {
            return 1;
        }
        else{
            uthread_yield();
        }
    }

}
void uthread_init(uthread_func_t func, void *arg) {
    tcbQueue = queue_create();
    zombieQueue = queue_create();
    struct Tcb* main = malloc(sizeof(struct Tcb));
    main->tid = 0;
    main->stack = uthread_ctx_alloc_stack();
    uthread_ctx_init(&main->ctx, main->stack, func, arg);
    currTcb = main;
}

