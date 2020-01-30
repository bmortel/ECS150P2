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

/*int queue_destroy(queue_t queue)
{
	*//* TODO Phase 1 *//*
}*/

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
    struct ListNode* tempHead = queue->head->next;
    // If data or queue is empty, return -1
    if ((data) == NULL || queue->head == NULL || tempHead == NULL){
        return -1;
    }

    // Assign the oldest item in the queue to the data pointer
    (*data) = queue->head->item;
    free(queue->head);
    queue->head = tempHead;
	queue->length--;

	return 0;
}

int queue_delete(queue_t queue, void *data)
{
    struct ListNode* curr = queue->head;
    struct ListNode* prev = NULL;
	if ((data) == NULL || queue->head == NULL) {
	     return -1;
	}

	while(curr->next != NULL) {
	    if ((data) == ((curr->item))) {
            if (prev == NULL) {
                queue_dequeue(queue, NULL);
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
	 /* TODO Phase 1 */
}

int queue_length(queue_t queue)
{
	 /* TODO Phase 1 */
}

int main() {
    queue_t q;
    int data = 3, *ptr;

    q = queue_create();
    printf("%d\n",queue_enqueue(q, &data));
    printf("%d\n",queue_dequeue(q, (void**)&ptr));


    return 0;
};

