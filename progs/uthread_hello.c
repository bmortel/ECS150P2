/*
 * Simple hello world test
 *
 * Tests the creation of a single thread and its successful return.
 */

#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>

int thread1(void* arg){
    printf("thread1\n");
    return5;
}

int main(void){
    int ret;
    uthread_t tid;
    tid = uthread_create(thread1, NULL);
    uthread_join(tid, &ret);
    printf("thread1 returned %d\n", ret);
    return0;
}