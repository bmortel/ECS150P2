#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "context.h"
#include "preempt.h"
#include "queue.h"
#include "uthread.h"
#include "ThreadControlBlock.h"

uthread_t TIDCount = 0;
struct Tcb* tcb;
/* TODO Phase 2 */

void uthread_yield(void)
{
	/* TODO Phase 2 */
}

uthread_t uthread_self(void)
{
	return tcb->tid;
}

int uthread_create(uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */

    tcb = malloc(sizeof(struct Tcb));
	tcb-> stack = uthread_ctx_alloc_stack();
    if (uthread_ctx_init(&tcb->ctx, tcb->stack, func, arg) == -1) {
        return -1;
    }
    TIDCount++;
    tcb->tid = TIDCount;
	return TIDCount;
}

void uthread_exit(int retval)
{
	/* TODO Phase 2 */
}

int uthread_join(uthread_t tid, int *retval)
{
    while(1) {

    }
	/* TODO Phase 2 */
	/* TODO Phase 3 */
}

