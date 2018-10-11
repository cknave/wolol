bits 64

extern exit, main

section .text
    global _start

_start:
    mov rdi, [rsp]
    lea rsi, [rsp+8]
    call main
    mov rdi, rax
    jmp exit
