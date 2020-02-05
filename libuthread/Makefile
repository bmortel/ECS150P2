# Target library
lib     := libuthread.a
objs    := queue.o uthread.o context.o preempt.o

CC      := gcc
CFLAGS  := -Wall -Wextra -Werror
CFLAGS  += -g

all: $(lib)

deps := $(patsubst %.o, %.d, $(objs))
-include $(deps)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<

libuthread.a: queue.o uthread.o context.o preempt.o
	ar rcs $(lib) $(objs)

clean:
	rm -f $(lib) $(objs) $(deps)