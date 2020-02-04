//
// Created by Justin Lim on 2/1/20.
//

#ifndef QUEUE_THREADCONTROLBLOCK_H
#define QUEUE_THREADCONTROLBLOCK_H

struct Tcb {
    uthread_t tid;
    ucontext_t* ctx;
    void* stack;
    char* state;
};


#endif //QUEUE_THREADCONTROLBLOCK_H
