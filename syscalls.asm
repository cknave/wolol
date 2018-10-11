bits 64

section .text
    global open, write, close, exit, socket, setsockopt, bind, sendto

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

socket:
    mov rax, 29h
    syscall
    ret

setsockopt:
    mov rax, 36h
    mov r10, rcx
    syscall
    ret

bind:
    mov rax, 31h
    syscall
    ret

sendto:
    mov rax, 2ch
    mov r10, rcx
    syscall
    ret
