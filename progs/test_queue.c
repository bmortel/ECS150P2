#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LinkedList.h"
#include "mcheck.h"
#include "queue.h"

#define TEST_ASSERT(assert)                     \
do {                                            \
        printf("ASSERT: " #assert " ... ");     \
        if (assert) {                           \
                printf("PASS\n");               \
        } else  {                               \
                printf("FAIL\n");               \
                exit(1);                        \
        }                                       \
} while(0)

/* Callback function that increments items by a certain value */
static int inc_item(void *data, void *arg) {
  int *a = (int *)data;
  int inc = (int)(long)arg;

  *a += inc;

  return 0;
}

/* Callback function that finds a certain item according to its value */
static int find_item(void *data, void *arg) {
  int *a = (int *)data;
  int match = (int)(long)arg;

  if (*a == match) return 1;

  return 0;
}

void test_create(void) {
  queue_t q;

  q = queue_create();
  TEST_ASSERT(q != NULL);
}

void test_queue_simple(void) {
  queue_t q;
  int data = 3, *ptr;

  q = queue_create();
  queue_enqueue(q, &data);
  queue_dequeue(q, (void **)&ptr);
  TEST_ASSERT(ptr == &data);
}

void test_iterator(void) {
  queue_t q;
  int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int i;
  int *ptr;

  /* Initialize the queue and enqueue items */
  q = queue_create();
  for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
    queue_enqueue(q, &data[i]);

  /* Add value '1' to every item of the queue */
  queue_iterate(q, inc_item, (void *)1, NULL);
  TEST_ASSERT(data[0] == 2);

  /* Find and get the item which is equal to value '5' */
  ptr = NULL;
  queue_iterate(q, find_item, (void *)5, (void **)&ptr);
  TEST_ASSERT(ptr != NULL);
  TEST_ASSERT(*ptr == 5);
  TEST_ASSERT(ptr == &data[3]);
}

void test_iterator_emptyQ() {
  queue_t q = queue_create();
  int retval, *ptr;
  retval = queue_iterate(q, find_item, (void *)5, (void **)&ptr);
  TEST_ASSERT(retval == 0);
}

void test_iterator_nullQ() {
  queue_t q = queue_create();
  queue_destroy(q);
  q = NULL;
  int retval, *ptr;
  retval = queue_iterate(q, find_item, (void *)5, (void **)&ptr);
  TEST_ASSERT(retval == -1);
}

void test_iterator_noFunc() {
  queue_t q = queue_create();
  int *ptr;
  int retval = queue_iterate(q, NULL, (void *)2, (void **)&ptr);
  TEST_ASSERT(retval == -1);
}

void test_enqueue_nullData() {
  queue_t q = queue_create();
  TEST_ASSERT(queue_enqueue(q, NULL) == -1);
}

void test_enqueue_nullQueue() {
  queue_t q = NULL;
  int data = 69;
  TEST_ASSERT(queue_enqueue(q, &data) == -1);
}

void test_delete(void) {
  queue_t q;
  q = queue_create();
  int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int i;
  int data = 5;

  for (i = 0; i < sizeof(array) / sizeof(array[0]); i++)
    queue_enqueue(q, &array[i]);

  queue_enqueue(q, &data);
  TEST_ASSERT(queue_length(q) == 11);
  queue_delete(q, &data);
  TEST_ASSERT(queue_length(q) == 10);
}

void test_delete_NULLQ(void) {
  queue_t q;
  q = queue_create();

  int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int i;

  for (i = 0; i < sizeof(array) / sizeof(array[0]); i++)
    queue_enqueue(q, &array[i]);

  queue_destroy(q);
  q = NULL;

  TEST_ASSERT(queue_delete(q, &array[4]) == -1);
}

void test_delete_NULLData(void) {
  queue_t q;
  q = queue_create();

  int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int i;

  for (i = 0; i < sizeof(array) / sizeof(array[0]); i++)
    queue_enqueue(q, &array[i]);

  TEST_ASSERT(queue_delete(q, NULL) == -1);
}

void test_delete_EmptyQ(void) {
  queue_t q;
  q = queue_create();
  int data = 5;

  TEST_ASSERT(queue_delete(q, &data) == -1);
}

void test_destroy() {
  queue_t q;
  q = queue_create();

  int j = 42;

  queue_enqueue(q, &j);

  TEST_ASSERT(queue_length(q) == 1);
  TEST_ASSERT(queue_destroy(q) == -1);

  queue_delete(q, &j);
  TEST_ASSERT(queue_length(q) == 0);

  queue_destroy(q);

  // free should throw error if queue_destroy succeeds
  // free(q);
}

void test_destroy_nonemptyQ() {
  queue_t q;
  q = queue_create();

  int j = 42;

  queue_enqueue(q, &j);

  TEST_ASSERT(queue_length(q) == 1);
  TEST_ASSERT(queue_destroy(q) == -1);
}

void test_destroy_nullQ() {
  queue_t q = NULL;
  TEST_ASSERT(queue_destroy(q) == -1);
}

void test_queue_length() {
  queue_t q = queue_create();
  int data[] = {1, 2};
  TEST_ASSERT(queue_length(q) == 0);
  queue_enqueue(q, &data[0]);
  TEST_ASSERT(queue_length(q) == 1);
  queue_enqueue(q, &data[1]);
  TEST_ASSERT(queue_length(q) == 2);
}

void test_queue_length_nullQ() {
  queue_t q = queue_create();
  int data[] = {1, 2};
  TEST_ASSERT(queue_length(q) == 0);
  queue_enqueue(q, &data[0]);
  TEST_ASSERT(queue_length(q) == 1);
  queue_enqueue(q, &data[1]);
  TEST_ASSERT(queue_length(q) == 2);

  queue_destroy(q);
  q = NULL;

  TEST_ASSERT(queue_length(q) == -1);
}

void test_dequeue_emptyQ() {
  queue_t q = queue_create();
  int data = 2;
  TEST_ASSERT(queue_dequeue(q, (void *)&data) == -1);
}

void test_dequeue_nullData(void) {
  queue_t q;
  int i = 3;
  q = queue_create();
  queue_enqueue(q, &i);
  TEST_ASSERT(queue_dequeue(q, NULL) == -1);
}

void test_dequeue_nullQ(void) {
  queue_t q;
  int data = 3, *ptr;
  q = queue_create();
  queue_enqueue(q, &data);
  queue_destroy(q);
  q = NULL;
  TEST_ASSERT(queue_dequeue(q, (void **)&ptr) == -1);
}

int main() {
  test_create();
  test_delete_EmptyQ();
  test_delete_NULLData();
  test_delete_NULLQ();
  test_delete();
  test_dequeue_emptyQ();
  test_dequeue_nullData();
  test_dequeue_nullQ();
  test_destroy();
  test_destroy_nonemptyQ();
  test_destroy_nullQ();
  test_enqueue_nullData();
  test_enqueue_nullQueue();
  test_iterator();
  test_iterator_emptyQ();
  test_iterator_noFunc();
  test_iterator_nullQ();
  test_queue_length();
  test_queue_length_nullQ();
  test_queue_simple();
}
