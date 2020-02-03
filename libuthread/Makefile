# Target library
lib := libuthread.a

all: $(lib)

queue.o: queue.c queue.h
	gcc -c queue.c queue.h

uthread.o: uthread.c uthread.h
	gcc -c uthread.c uthread.h

context.o: context.c context.h
	gcc -c context.c context.h

libuthread.a: queue.o uthread.o context.o
	ar rcs $(lib) queue.o uthread.o context.o

clean:
	rm -f *.o *.a *.gch