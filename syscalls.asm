bits 64

section .text
    global open, write, close, exit

open:
    mov rax, 2h
    syscall
    ret

write:
    mov rax, 1h
    syscall
    ret

close:
    mov rax, 3h
    syscall
    ret

exit:
    mov rax, 3ch
    syscall
