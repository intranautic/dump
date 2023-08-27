; studying cpuid based on intel manual
section .text
global _start
_start:
  cpuid
  pushfq
  .exit:
    mov rax, 60
    syscall
