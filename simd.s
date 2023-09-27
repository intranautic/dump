; x86 vector processing extensions
; check for processor support in /proc/cpuinfo

section .rodata
  d1: dd 1, 2, 3, 4
  d2: dd 5, 6, 7, 8
  q1: dq 1, 2, 3, 4
  q2: dq 5, 6, 7, 8

section .text
global _start
_start:
  ; 32 sse and avx only if avx512 is supported else 16
  ; sse    128 bit: xmm0-xmm31
  ; avx    256 bit: ymm0-ymm31
  ; avx512 512 bit: zmm0-zmm31
  ; [    zmm0: 512    | ymm0: 256 | xmm0: 128]

  ; xmm0: [32 bits: 0x1][32 bits: 0x2][32 bits: 0x3][32 bits: 0x4]
  ; xmm1: [32 bits: 0x5][32 bits: 0x6][32 bits: 0x7][32 bits: 0x8]
  movdqu xmm0, [d1]
  movdqu xmm1, [d2]

  paddd xmm0, xmm1
  nop ; break
  .exit:
    mov rax, 60
    syscall
