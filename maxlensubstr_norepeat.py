#!/usr/bin/env python3
from sys import argv

if __name__ == '__main__':
  try:
    i=argv[1] # dont want to wrap in func
    s, e, l = 0, 0, 0
    u = set()
    while e < len(i):
      if i[e] not in u:
        u.add(i[e])
        e += 1
        l = max(l, len(u))
      else:
        u.remove(i[s])
        s += 1
    print(l)
  except IndexError:
    print("Usage: %s <string>" % argv[0])
