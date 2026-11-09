
global _start

section .text

_start:
    mov eax, 4
    mov ebx, 1
    mov ecx, message
    mov edx, message_length
    int 0x80

    mov eax, 0x1
    mov ebx, 0
    int 0x80

section .data
    message: db "Hello, world!", 0xA
    message_length equ $-message
