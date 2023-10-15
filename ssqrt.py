#!/usr/bin/env python3
from sys import argv

def ssqrt(n: int, s: int):
  for i in range(2, n):
    d, m = divmod(n, i**s)
    if not m:
      p, q = ssqrt(d, s)
      return (p*i, q)
    if not d:
      break
  return (1, n)

if __name__ == '__main__':
  try:
    x = ssqrt(int(argv[1]), int(argv[2]))
    print(x)
  except IndexError:
    print("Usage: %s <number> <degree>" % argv[0])
