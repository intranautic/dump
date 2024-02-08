#!/usr/bin/env python3
import angr
import multiprocessing
import argparse
import logging
from sys import argv
from os import listdir
from tqdm import tqdm
from functools import partial

# flip_boolean_cmp
# simplify_else_scope

def angr_decompile_all(
    path: str,
    outdir: str,
  ) -> str:
  project = angr.Project(path, inst_path=outdir, auto_load_libs=False)
  buffer=''
  try:
    cfg = project.analyses.CFGFast(normalize=True)
  except Exception as e:
    print(e)
    return

  for func in cfg.functions.values():
    try:
      code = project.analyses.Decompiler(
        func,
        cfg = cfg.model,
      ).codegen
      if code is not None:
        buffer += code.text
    except Exception as e:
      print(e)
  project.inst_file.flush()
  return buffer

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
          partial(angr_decompile_all, outdir=args.output),
          paths,
        ),
        total=len(paths)
    ):
      print(result)

    pool.close()
    pool.join()
