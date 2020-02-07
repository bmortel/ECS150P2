# Project #2 - User-level thread library
## Queue API

The queue is implemented by using a linked list. A header file LinkedList.h 
defines the linked list structure. 

The linked list is comprised of `ListNode` structs that contain a `void*` 
`item` to hold data and `ListNode* next` to point to the next node. 

The queue struct contains two `ListNode` pointers `head` and `curr` to 
keep track of the beginning and end of the list and an `int length` to 
quickly get the length of the list.

Functions for the queue are performed easily and quickly by manipulating the 
`ListNode` pointers

A linked list is effective for building a queue as the addition and removal of 
nodes is _O(1)_.
### Testing the queue
The queue is tested by a program called `queue_tester` to make sure it is 
resistant. `queue_tester` contains 20 different test cases that attempt 
every possible usage scenario for each function of the API and checks for the 
correct output.

----------
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
The library also has helped functions such as `check_tid` which searched for a 
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
### uthread_exit 
When the exit thread is ran, the current threads state is 
changed into zombie and enqueued to the zombie queue. If the thread is a child 
thread and joining a parent thread, it iterates through the blocked queue to 
find the parent thread. The parent thread is enqueued to the ready queue and 
deleted from the blocked queue. Then, the function dequeues the next ready 
thread and performs a context switch. 
### testing
To test our uthread library, we used the provided `utest_hello.c` and
`utest_yield.c` programs provided. Then we modified the programs to 
test other specifications and cover more cases.

-----
## Preemption
Preemption is implemented by using `sigaction` from `signal.h` and 
`itimer` from `sys/time.h`.

Two `sigaction` structs are defined ... `oldAction` to store the default 
`SIGVTALRM` signal action and `sigActionStruct` to reference 
the new signal handler to be used for preemption. 

An itimer struct `timer` is declared to be used as the alarm. 

The signal handler `signalHandler()` calls `uthread_yield()` 
### preempt initialization
The function `preempt_start()` installs the signal handler and set up an 
alarm to fire SIGVTALRM signals. It is called by `uthread_create()` when 
doing the main thread initialization.

The signal handler is installed by modifying `sigActionStruct` s `sa_mask` 
and `sa_handle`r fields. `sigemptyset()` is called on `sa_mask` to clear 
any garbage code. `sa_handler` is set to `signalHandler`. Then `sigaction
(SIGVTALRM, &sigActionStruct, &oldAction)` is called to set `SIGVTALRM` 
signal handling to `sigActionStruct` and store the old `SIGVTALRM` handler 
in `oldAction`.

The alarm is implemented by setting `timer`'s `it_value.tv_sec` and 
`it_interval.tv_sec` members to 0 and setting `it_interval.tv_usec` and 
`it_value.tv_usec` to 10000. 10000 usec will result in 100HZ. Then 
`setitimer(ITIMER_VIRTUAL, &timer, NULL)
` is called to set a virtual timer using the `timer` struct values.
### preempt enabling/disabling
`preempt_enable()` and `preempt_disable()` both use `sigaction()` to 
install the new signal handler and restore the original signal 
handler respectively.

`sigaction()` is used because it is multithread safe and asynchronous safe.
### testing preemption
The program `test_preempt` tests the preemption implementation. In the 
program `main()` creates and joins `thread1` which creates and joins 
`thread2` which creates and joins `thread3`. `thread1`, `thread2`, and 
`thread3` increment global integer variables `a`, `b`, and `c` 
respectively until the value 999999. The values of `a`, `b`, and `c` are
printed in the terminal.

This program demonstrates preemption because one can observe the variables 
taking turns being incremented. Without preemption each thread would run until 
it is done incrementing before allowing the next one to run.

# Sources 
ECS 150 Slides   
[Stack Overflow](https://stackoverflow.com/)  
[Geeks for Geeks](https://www.geeksforgeeks.org/)  
[Linux man pages](https://linux.die.net/man/)
