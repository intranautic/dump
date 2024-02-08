#!/usr/bin/env python3
import argparse
from sys import argv
import subprocess
import multiprocessing
import logging
import os
import pathlib
from tqdm import tqdm


def ida_decompile_all(path: str) -> None:
  binary_path = pathlib.Path(path).absolute()
  binary_name = binary_path.with_suffix('').name
  ida_path = os.getenv("IDA_PATH") or "idat64"
  dec_cmd = f'{ida_path} -A -c -Ohexrays:ida_{binary_name}:ALL {binary_path}'
  subprocess.run([dec_cmd], timeout=300, shell=True)
  return

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
      os.listdir(args.directory) if file.endswith(args.extension)  
    ]
    progress = tqdm(
        pool.imap_unordered(
        ida_decompile_all,
        paths,
      ),
      total=len(paths)
    )
    for result in progress:
      print(result)

    progress.close()
    pool.close()
    pool.join() 
