section .text 
global _start
_start:
  pushf
  or qword [rsp], 0x100
  popf
  .exit:
    mov rax, 60
    xor rdi, rdi
    syscall
