#!/usr/bin/env python3
import angr
from angr.analyses import Decompiler
from sys import argv
from typing import List

def disable_decompiler_option(params: List[str], values):
    return list(zip(
      [
        opt for opt in angr.analyses.decompiler.decompilation_options.options
        if opt.param in params
      ],
      values,
    ))

# return "pretty" decompiler output
# non flexible angr decompiler wrapper for testing purposes
def angr_decompile(
    project: angr.Project,
    symbols: [str] = None
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
        options=disable_decompiler_option(['cstyle_ifs'], [True])
      ).codegen.text
  else:
    for func in cfg.functions.values():
      print(func)
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

