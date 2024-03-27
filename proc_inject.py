#!/usr/bin/env python3
from pwn import *
from sys import argv
from functools import reduce
from enum import Enum


class page_attr(Enum):
  READ    = 1
  WRITE   = 1 << 1
  EXECUTE = 1 << 2
  PRIVATE = 1 << 3

class mm_region(object):
  __slots__ = (
    'memmap', 'addr_base', 'addr_end', 'addr_limit', 'perm_attr',
    'offset', 'devmin', 'devmax', 'inode', 'name',
  )
  memmap:     int
  addr_base:  int
  addr_end:   int
  addr_limit: int
  perm_attr:  page_attr
  offset:     int
  devmin:     int
  devmax:     int
  inode:      int
  name:       str

  def __init__(self, memmap: str):
    self.memmap = memmap
    region = list(filter(str.strip, self.memmap.split(' ')))
    self.addr_base, self.addr_end = tuple(map(int, region[0].split('-')))
    self.addr_limit = self.addr_end - self.addr_base
    self.perm_attr = reduce(lambda x,y:x.value+y.value, list(map(lambda perm: {
      'r': page_attr.READ, 
      'w': page_attr.WRITE,
      'x': page_attr.EXECUTE,
      'p': page_attr.PRIVATE,
    }[perm], list(region[1].replace('-', '')))))
    print(self.perm_attr)

  def dump(self) -> None:
    pass


if __name__ == '__main__':
  try:
    maps = open('/proc/{}/maps'.format(argv[1])) \
      .read() \
      .split('\n')

    x = mm_region(maps[0])


  except IndexError:
    print("Usage: %s <pid>" % argv[0])
