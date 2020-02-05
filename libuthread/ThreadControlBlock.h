//
// Created by Justin Lim on 2/1/20.
//

#ifndef QUEUE_THREADCONTROLBLOCK_H
#define QUEUE_THREADCONTROLBLOCK_H

enum state{ready, running, blocked, zombie};

struct Tcb {
    uthread_t tid;
    ucontext_t ctx;
    void* stack;
    enum state curState;
    int retval;
    struct* Tcb joined;
};




#endif //QUEUE_THREADCONTROLBLOCK_H
