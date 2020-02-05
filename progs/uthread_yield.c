/*
 * Thread creation and yielding test
 *
 * Tests the creation of multiples threads and the fact that a parent thread
 * should get returned to before its child is executed. The way the printing,
 * thread creation and yielding is done, the program should output:
 *
 * thread1
 * thread2
 * thread3
 */

#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>

uthread_t tid[2];

int thread2(void *arg) {
    int ret;
    printf("thread2\n");
    uthread_join(tid[0], &ret);
    printf("thread1 returned %d\n", ret);
    return 2;
}

int thread1(void *arg) {
    tid[1] = uthread_create(thread2, NULL);
    printf("thread1\n");
    return 1;
}

int main(void) {
    int ret;
    tid[0] = uthread_create(thread1, NULL);
    uthread_yield();
    uthread_join(tid[1], &ret);
    printf("thread2 returned %d\n", ret);
    return 0;
}

