NASM = nasm
CFLAGS = -O2

wlolol: wlolol.o syscalls.o
	$(CC) $(CFLAGS) -s -static -nostdlib -nostartfiles -o $@ $<

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

.asm.o:
	$(NASM) $(NASM_FLAGS) -o $@ $<
