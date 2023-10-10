; nasm -f elf64 user_input.s && ld -o user_input user_input.o
section .rodata
  msg: db "What is your name: ", 0
  msg_len: equ $ - msg
  out: db "Nice to meet you ", 0
  out_len: equ $ - out

section .text
global _start
_start:
  ; jump into main and push return address onto stack
  call main
  ; exit
  mov rax, 60
  syscall

main:
  ; setup a new stack frame
  push rbp
  mov rbp, rsp
  sub rsp, 0x30

  ; print enter prompt to stdout (1)
  mov rax, 1
  mov rdi, 1
  mov rsi, msg
  mov rdx, msg_len
  syscall

  ; read name from stdin (0)
  mov rax, 0
  mov rdi, 0
  lea rsi, qword [rbp-0x20]
  mov rdx, 0x20
  syscall

  ; print out "Nice to meet you "
  mov rax, 1
  mov rdi, 1
  mov rsi, out
  mov rdx, out_len
  syscall

  ; print out the input
  mov rax, 1
  mov rdi, 1
  lea rsi, qword [rbp-0x20]
  mov rdx, 0x20
  syscall

  ; restore previous stack frame
  leave
  ret ; return (pop -> rip)

