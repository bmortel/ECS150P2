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
    myQueue->head = (struct ListNode *) malloc(sizeof(struct ListNode));
    myQueue->head->next = NULL;
    myQueue->head->item = NULL;
    myQueue->curr = myQueue->head;
	myQueue->length = 0;
	return myQueue;
}

int queue_destroy(queue_t queue)
{
	if (queue->length != 0 || queue == NULL){
        return -1;
    }
    free(queue);
    return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
    struct ListNode* nextN = (struct ListNode *) malloc(sizeof(struct ListNode));
    nextN->next = NULL;
    nextN->item = data;
    if (nextN == NULL || data == NULL) {
        return -1;
    }

	queue->curr->next = nextN;
	queue->curr = queue->curr->next;
	queue->length++;
    
	return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
    struct ListNode* tempHead = queue->head->next->next;
    // If data or queue is empty, return -1
    if ((data) == NULL || queue->head->next == NULL || queue == NULL){
        return -1;
    }

    // Assign the oldest item in the queue to the data pointer
    (*data) = queue->head->next->item;
    free(queue->head->next);
    queue->head->next = tempHead;
	queue->length--;

	return 0;
}

int queue_delete(queue_t queue, void *data)
{
    struct ListNode* curr = queue->head->next;
    struct ListNode* prev = NULL;
	if ((data) == NULL || queue->head == NULL) {
	     return -1;
	}

	while(curr != NULL) {
	    if ((data) == ((curr->item))) {
            if (prev == NULL) {
                queue->head->next = curr->next;
                free(curr);
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

    struct ListNode* curr = queue->head->next;

    while(curr != NULL){
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

/*
int main() {
    queue_t q;
    int data = 3, *ptr;

    q = queue_create();
    printf("%d\n",queue_enqueue(q, &data));
    printf("%d\n",queue_dequeue(q, (void**)&ptr));


    return 0;
};
*/
