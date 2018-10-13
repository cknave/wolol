CFLAGS = -Wall -Os -fno-stack-protector
# CFLAGS = -Wall -O0 -g -fno-stack-protector
LDFLAGS = -s
NASM = nasm
NASM_FLAGS = -f elf64

OBJS = start.o syscalls.o wolol.o

wolol: wolol.bin
	objcopy --remove-section=.comment $< $@

wolol.bin: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

clean:
	rm -f $(OBJS) wolol wolol.bin

wolol.o: wolol.c syscalls.h

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.asm
	$(NASM) $(NASM_FLAGS) -o $@ $<
