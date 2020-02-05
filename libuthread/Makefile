# Target library
lib     := libuthread.a
objs    := queue.o uthread.o context.o preempt.o

CC      := gcc
CFLAGS  := -Wall -Wextra -Werror
CFLAGS  += -g

all: $(lib)

queue.o: queue.c queue.h
	$(CC) $(CFLAGS) queue.c queue.h

uthread.o: uthread.c uthread.h
	$(CC) $(CFLAGS) uthread.c uthread.h

context.o: context.c context.h
	$(CC) $(CFLAGS) context.c context.h

preempt.o: preempt.c preempt.h
	$(CC) $(CFLAGS) preempt.c preempt.h

libuthread.a: queue.o uthread.o context.o preempt.o
	ar rcs $(lib) $(objs)

clean:
	rm -f *.o *.a *.gch *.h.gch