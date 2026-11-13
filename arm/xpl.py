#!/usr/bin/env python3
from pwn import *
context.arch = 'aarch64'

payload = asm("""
  movz x2, #0x4000
  movk x2, #0x4000, lsl #16
  movk x2, #0x1337, lsl #32
  ldp x0, x1, [x2]
  add x3, x0, x1
  str x3, [x2, #0x10]!
""")

with process('/challenge/run') as p:
  p.send(payload)
  p.stdin.close()
  p.interactive()
