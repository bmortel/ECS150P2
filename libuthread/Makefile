# Target library
lib := libuthread.a

all: $(lib)

queue.o: queue.c queue.h
	gcc -c queue.c queue.h

uthread.o: uthread.c uthread.h
	gcc -c uthread.c uthread.h

context.o: context.c context.h
	gcc -c context.c context.h

preempt.o: preempt.c preempt.h
	gcc -c preempt.c preempt.h

libuthread.a: queue.o uthread.o context.o preempt.o
	ar rcs $(lib) queue.o uthread.o context.o preempt.o

clean:
	rm -f *.o *.a *.gch *.h.gch