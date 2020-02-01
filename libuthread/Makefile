# Target library
lib := libuthread.a

all: $(lib)

queue.o: queue.c queue.h
	gcc -c queue.c queue.h

libuthread.a: queue.o
	ar rcs $(lib) queue.o

clean:
	rm -f *.o *.a *.gch