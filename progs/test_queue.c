#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <malloc/malloc.h>
#include "queue.h"
#include "LinkedList.h"




void test_create(void)
{
    queue_t q;

    q = queue_create();
    assert(q != NULL);
}

void test_queue_simple(void)
{
    queue_t q;
    int data = 3, *ptr;

    q = queue_create();
    queue_enqueue(q, &data);
    queue_dequeue(q, (void**)&ptr);
    assert(ptr == &data);
}

/* Callback function that increments items by a certain value */
static int inc_item(void *data, void *arg)
{
    int *a = (int*)data;
    int inc = (int)(long)arg;

    *a += inc;

    return 0;
}

/* Callback function that finds a certain item according to its value */
static int find_item(void *data, void *arg)
{
    int *a = (int*)data;
    int match = (int)(long)arg;

    if (*a == match)
        return 1;

    return 0;
}

void test_iterator(void)
{
    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int i;
    int *ptr;

    /* Initialize the queue and enqueue items */
    q = queue_create();
    for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);

    /* Add value '1' to every item of the queue */
    queue_iterate(q, inc_item, (void*)1, NULL);
    assert(data[0] == 2);

    /* Find and get the item which is equal to value '5' */
    ptr = NULL;
    queue_iterate(q, find_item, (void*)5, (void**)&ptr);
    assert(ptr != NULL);
    assert(*ptr == 5);
    assert(ptr == &data[3]);
}

void test_delete(void){
    queue_t q;
    q = queue_create();
    int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int i;
    int data = 5;

    for (i = 0; i < sizeof(array) / sizeof(array[0]); i++)
        queue_enqueue(q, &array[i]);

    queue_enqueue(q, &data);
    assert(queue_length(q) == 11);
    queue_delete(q, &data);
    assert(queue_length(q) == 10);

}


void test_destroy(){

    //mallopt()

    queue_t q;
    q = queue_create();



    int j = 42;

    queue_enqueue(q, &j);

    assert(queue_length(q) == 1);
    assert(queue_destroy(q) == -1);

    queue_delete(q, &j);
    assert(queue_length(q) == 0);

    queue_destroy(q);

// free should throw error if queue_destroy succeeds 
    //free(q);

}



int main(){

    test_create();
    test_queue_simple();
    test_iterator();
    test_delete();
    test_destroy();
}