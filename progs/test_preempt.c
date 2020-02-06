#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <uthread.h>

int a;
int b;
int c;

int thread3(void* arg) {
  while (c < 99999) {
    printf("a:%d\tb:%d\tc:%d\n", a, b, c);
    ++c;
  }
  return 5;
}
int thread2(void* arg) {
  uthread_t tid = uthread_create(thread3, NULL);
  int ret;
  uthread_yield();
  while (b < 99999) {
    printf("a:%d\tb:%d\tc:%d\n", a, b, c);
    ++b;
  }
  uthread_join(tid, &ret);
  return 5;
}

int thread1(void* arg) {
  uthread_t tid = uthread_create(thread2, NULL);
  int ret;
  uthread_yield();
  while (a < 99999) {
    printf("a:%d\tb:%d\tc:%d\n", a, b, c);
    ++a;
  }
  uthread_join(tid, &ret);
  return 5;
}

int main(void) {
  int ret;
  uthread_t tid;
  tid = uthread_create(thread1, NULL);
  uthread_join(tid, &ret);
  return 0;
}
