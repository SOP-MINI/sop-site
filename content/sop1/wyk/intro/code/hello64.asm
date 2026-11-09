global _start

section .text

_start:
  mov rax, 1
  mov rdi, 1
  mov rsi, message
  mov rdx, message_length
  syscall

  mov rax, 60
  mov rdi, 0
  syscall

section .data
  message: db "Hello, world!", 0xA
  message_length: equ $-message
