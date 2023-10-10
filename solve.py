#!/usr/bin/env python3
from sys import argv

if __name__ == "__main__":
  try:
    print(''.join(sorted([*set(argv[1])])))
  except IndexError:
    print("Usage: %s <string>"%argv[0])
