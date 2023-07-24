#!/usr/bin/env python3
import angr
from angr.analyses import Decompiler
from sys import argv
from typing import List, Tuple
import logging

def set_decompiler_option(decompiler_options: List[Tuple], params: List[Tuple]) -> List[Tuple]:
  if decompiler_options is None:
    decompiler_options = []

  for param, value in params:
    for option in angr.analyses.decompiler.decompilation_options.options:
      if param == option.param:
        decompiler_options.append((option, value))

  return decompiler_options

# return "pretty" decompiler output
# non flexible angr decompiler wrapper for testing purposes
def angr_decompile(
    project: angr.Project,
    symbols: [str] = None,
    decompiler_options=None
  ) -> str:
  buffer=''
  try:
    cfg = project.analyses.CFGFast(normalize=True)
  except Exception as e:
    print(e)
  if symbols:
    for func in symbols:
      buffer += project.analyses.Decompiler(
        cfg.functions[func],
        cfg = cfg.model,
        kb = cfg.kb,
      ).codegen.text
  else:
    for func in cfg.functions.values():
      codegen = project.analyses[Decompiler].prep()(
        func,
        cfg=cfg.model,
      ).codegen
      if codegen is not None:
        buffer += codegen.text
  return buffer

if __name__ == '__main__':
  try:
    d = angr_decompile(angr.Project(argv[1], auto_load_libs=False), argv[2::])
    print(d)
  except IndexError:
    print('Usage: %s <binary> <symbol>' % argv[0])

