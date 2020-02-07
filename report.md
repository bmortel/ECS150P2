# Project 1: Simple Shell Report 

## uthread API 

### Implementation 
To implement our uthread library, we took advantage of the 
queue API we have developed earlier. In order to store information about 
individual threads, we created a data structure called `Tcb` or Thread Control 
Block. Each thread had a corresponding `Tcb` and held the following data: 
* The TID of the thread 
* The context of the thread 
* Pointer to the head of the stack 
* The state the thread is currently in 
* The return value of the thread
* A boolean to check if the thread is joining 
* The TID of the parent thread if the thread is joining 

In order to store the `Tcb` of each thread, we had three queues in our library 
for the ready threads, the zombie threads, and the blocked threads. The library 
also has a boolean value `init` to check if the main thread had been 
initialized. If the main thread hadn't been initialized, the thread is given a 
TID of 0. All the queues are also initialized when the main thread is 
initialized. There is also a global variable that keeps count of the TIDs and 
increments by one every time a new thread is created. A `Tcb` variable called `
currTcb` is used to store the currently running thread. 

The library also has helper functions such as `check_tid` which searches for a 
given thread with the matching given TID, `check_waiting` which searches for 
the parent TID stored in the thread in a given queue, and `free_claimed` which 
frees the memory of a thread after it has been collected. 

### uthread_create 
The join function first checks if the main thread has been initialized. 
Afterwards, it creates a new `Tcb` variable and allocates memory for the stack. 
The TID is incremented and assigned to the newly created thread. The current 
state of the thread is assigned to ready and is enqueued to the ready queue. 

### uthread_yield 
The yield function first checks if the current thread is 
blocked. If the thread is not blocked, then the queue can be added to the ready 
queue. The function then attempts to dequeue the next ready thread from the 
ready queue if there is one and performs a context switch. 

### uthread_join 
The join function first checks if the given TID is valid(not 0 
or negative). Then it switches the current thread's state into blocked and the 
thread is added to the blocked queue. The function searched through the ready 
queue and the zombie queue for the given thread. The zombie queue is also 
searched because the child thread could have exited already but not collected. 
The function then attempts to delete the child thread in the zombie thread if 
it is in zombie state. The TID of the parent thread is assigned to the child 
thread. The function yields until the child is exited. The return value of the 
child thread is assigned to the address of `retval` passed through the 
parameters. Finally, the child thread is claimed and is freed. 

### uthread_exit When the exit thread is ran, the current threads state is 
changed into zombie and enqueued to the zombie queue. If the thread is a child 
thread and joining a parent thread, it iterates through the blocked queue to 
find the parent thread. The parent thread is enqueued to the ready queue and 
deleted from the blocked queue. Then, the function dequeues the next ready 
thread and performs a context switch. 

# Sources 
ECS 150 Slides   
[Stack Overflow](https://stackoverflow.com/)  
[Geeks for Geeks](https://www.geeksforgeeks.org/)  
[Linux man pages](https://linux.die.net/man/)
