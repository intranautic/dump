#!/usr/bin/env python3
import angr
import multiprocessing
import argparse
import logging
from sys import argv
from os import listdir
from tqdm import tqdm
from functools import partial
from binaryninja import *


def binja_decompile_all(path, outdir):
  lines = ''
  settings = DisassemblySettings()
  settings.set_option(DisassemblyOption.ShowAddress, False)	
  with open_view(path) as bv:
    obj = lineardisassembly.LinearViewObject.language_representation(bv, settings)
    cursor_end = lineardisassembly.LinearViewCursor(obj)
    for func in bv.functions:
      cursor_end.seek_to_address(func.highest_address)
      body = bv.get_next_linear_disassembly_lines(cursor_end)
      cursor_end.seek_to_address(func.highest_address)
      header= bv.get_previous_linear_disassembly_lines(cursor_end)
      for line in header:
          lines += str(line) + '\n'
      for line in body:
          lines += str(line) + '\n'
  open(outdir +'/'+ 'binja_' +os.path.basename(path)+'.c', 'a+').write(lines+'\n')
  return lines

if __name__ == '__main__':
  parser = argparse.ArgumentParser(
    prog = argv[0],
    description = 'Benchmark instrumented invocations',
  )
  parser.add_argument('-d', '--directory', default='.')
  parser.add_argument('-c', '--cores', default=1)
  parser.add_argument('-e', '--extension', default='.o')
  parser.add_argument('-o', '--output')
  args = parser.parse_args()

  logging.disable(logging.CRITICAL)
  with multiprocessing.Pool(processes=int(args.cores)) as pool:
    paths = [
      args.directory+'/'+file for file in
      listdir(args.directory) if file.endswith(args.extension)  
    ]
    for result in tqdm(
        pool.imap_unordered(
          partial(binja_decompile_all, outdir=args.output),
          paths,
        ),
        total=len(paths)
    ):
      print(result)

    pool.close()
    pool.join()
