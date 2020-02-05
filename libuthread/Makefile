# Target library
lib     := libuthread.a
objs    := queue.o uthread.o context.o preempt.o

CC      := gcc
CFLAGS  := -Wall -Wextra -Werror
CFLAGS  += -g

ifneq ($(V),1)
Q = @
endif

all: $(lib)

deps := $(patsubst %.o, %.d, $(objs))
-include $(deps)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<

libuthread.a: queue.o uthread.o context.o preempt.o
	ar rcs $(lib) $(objs)

clean:
    @echo "CLEAN"
	$(Q)rm -f $(lib) $(objs) $(deps)