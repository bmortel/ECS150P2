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
	if (queue == NULL || queue->length != 0){
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


	queue->curr->next = nextN;
	queue->curr = queue->curr->next;
	queue->length++;
    
	return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
    // If data or queue is empty, return -1
    if ((data) == NULL || queue == NULL || queue->head->next == NULL ){
        return -1;
    }
    struct ListNode* tempHead = queue->head->next->next;


    // Assign the oldest item in the queue to the data pointer
    (*data) = queue->head->next->item;
    free(queue->head->next);
    queue->head->next = tempHead;
	queue->length--;

	return 0;
}

int queue_delete(queue_t queue, void *data)
{
    if (queue == NULL || (data) == NULL || queue->head == NULL) {
	     return -1;
	}
    struct ListNode* curr = queue->head->next;
    struct ListNode* prev = NULL;


	while(curr != NULL) {
	    if ((data) == ((curr->item))) {
            // If @data found in first ListNode
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
