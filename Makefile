CC = gcc
CFLAGS = -g -O0 -fno-stack-protector
NASM = nasm
NASM_FLAGS = -f elf64

OBJS = start.o syscalls.o wlolol.o

wlolol: $(OBJS)
	$(CC) $(CFLAGS) -static -nostdlib -nostartfiles -o $@ $^

clean:
	rm -f $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.asm
	$(NASM) $(NASM_FLAGS) -o $@ $<
