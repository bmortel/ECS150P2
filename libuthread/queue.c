#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#include <ctype.h>


#include "queue.h"
#include "LinkedList.h"

struct queue {
    struct ListNode* head;
    int length;
    struct ListNode* curr;
};

queue_t queue_create(void)
{
	queue_t myQueue = (queue_t) malloc(sizeof(struct queue));
    myQueue->head = NULL;
    myQueue->curr = myQueue->head;
	myQueue->length = 0;
	return myQueue;
}

int queue_destroy(queue_t queue)
{
	if (queue == NULL || queue->length != 0|| queue->head == NULL){
        return -1;
    }
    free(queue->head);
    free(queue);
    
    return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
    if (queue == NULL || data == NULL) {
        return -1;
    }
    struct ListNode* nextN = (struct ListNode *) malloc(sizeof(struct ListNode));
    nextN->next = NULL;
    nextN->item = data;

    if (queue->head == NULL) {
        queue->head = nextN;
        queue->curr = queue->head;
    }
    else {
        queue->curr->next = nextN;
        queue->curr = queue->curr->next;
    }
    queue->length++;
    
	return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
    // If data or queue is empty, return -1
    if ((data) == NULL || queue == NULL || queue->head == NULL ){
        return -1;
    }
    if (queue->head->next == NULL) {
        (*data) = queue->head->item;
        queue->head = NULL;
    }
    else {
        struct ListNode *tempHead = queue->head->next;
        // Assign the oldest item in the queue to the data pointer
        (*data) = queue->head->item;
        free(queue->head);
        queue->head = tempHead;
        queue->length--;
    }

	return 0;
}

int queue_delete(queue_t queue, void *data)
{
    if (queue == NULL || (data) == NULL || queue->head == NULL) {
	     return -1;
	}
    struct ListNode* curr = queue->head;
    struct ListNode* prev = NULL;

	while(curr != NULL) {
	    if ((data) == ((curr->item))) {
            // If @data found in first ListNode
            if (prev == NULL) {
                prev = queue->head;
                queue->head = curr->next;
                free(prev);
                queue->length--;
            }
            else {
                prev->next = curr->next;
                free(curr);
                queue->length--;
            }
            return 0;
	    }
	    prev = curr;
	    curr = curr->next;
	}

	return -1;
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
    if (queue == NULL || func == NULL){
        return -1;
    }

    struct ListNode* curr = queue->head;

    // while not at the end of the list
    while(curr != NULL){
        // If @func returns 1 for current item and @data
        // is not NULL set data to item and stop iteration
        if(func(curr->item, arg)){
            if (data != NULL){
            *data = curr->item;
            }
            break;
        }
        curr = curr->next;
    }

    return 0;
}

int queue_length(queue_t queue)
{
    if (queue == NULL)
        return -1;
	return queue->length;
    
}


