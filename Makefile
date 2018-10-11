CC = gcc
CFLAGS = -s -Os -fno-stack-protector
# CFLAGS = -g -O0 -fno-stack-protector
NASM = nasm
NASM_FLAGS = -f elf64

OBJS = start.o syscalls.o wolol.o

wolol: $(OBJS)
	$(CC) $(CFLAGS) -static -nostdlib -nostartfiles -o $@ $^

clean:
	rm -f $(OBJS)

wolol.o: wolol.c syscalls.h

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.asm
	$(NASM) $(NASM_FLAGS) -o $@ $<
