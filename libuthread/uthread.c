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
    void* tcb = NULL;
    uthread_ctx_t* prev = currTcb->ctx;

    if (currTcb->curState != blocked) {
        queue_enqueue(readyQueue, currTcb);
    }
	if (queue_dequeue(readyQueue,tcb) != -1) {

        uthread_ctx_switch(prev, ((struct Tcb *) tcb)->ctx);
        currTcb = (struct Tcb *) tcb;
        currTcb->curState = running;
        printf("%d\n",currTcb->tid);
    }
}

uthread_t uthread_self(void)
{
	return currTcb->tid;
}

int uthread_create(uthread_func_t func, void *arg)
{
    if (!init) {
        uthread_init(func, arg);
    }
    printf("hi\n");
    struct Tcb* tb = malloc(sizeof(struct Tcb));
    printf("hi\n");
    tb->ctx = malloc(sizeof(uthread_ctx_t));
    printf("hi\n");
    tb->stack = uthread_ctx_alloc_stack();
    if (uthread_ctx_init(tb->ctx, tb->stack, func, arg) == -1) {
        return -1;
    }
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

    queue_enqueue(blockedQueue, currTcb);
    currTcb->curState = blocked;


    while(1) {

        if (queue_length(readyQueue) == 0) {
            break;
        }
        else{
            uthread_yield();
        }
    }

    while (queue_length(blockedQueue) != 0) {
        if (queue_dequeue(blockedQueue,tcb) != -1) {
            uthread_ctx_switch(NULL, ((struct Tcb *) tcb)->ctx);
            currTcb = (struct Tcb *) tcb;
            currTcb->curState = running;

        }
    }
    return 1;

}
void uthread_init(uthread_func_t func, void *arg) {

    readyQueue = queue_create();
    zombieQueue = queue_create();
    blockedQueue = queue_create();
    struct Tcb* main = (struct Tcb*)malloc(sizeof(struct Tcb));
    main->tid = 0;
    main->stack = uthread_ctx_alloc_stack();
    main->ctx = malloc(sizeof(uthread_ctx_t));
    currTcb = main;
}

/*
queue_func_t check_tid(void * tcb, uthread_t tid2) {
    if (((struct Tcb*)tcb)->tid == tid2) {
        return (queue_func_t) 1;
    }
}
*/

