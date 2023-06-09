from json import load, dumps
from sys import stdin

TERM = 'jmp', 'ret', 'call', 'br'

# TODO: ugly, refactor
def blocks_form(body):
  blocks = []
  current = []

  for instrs in body:
    if 'op' in instrs:
      current.append(instrs)
      if instrs['op'] in TERM:
        blocks.append(current)
        current = []
    else:
      blocks.append(current)
      current = [instrs]
  if current:
    blocks.append(current)
  return blocks

def cfg_construct(program):
  """
  formal representation of a directed control flow graph in the format:

  {
    {'nodes': [a, b, c]},
    {'edges': [(a, b), (b, c), (c, a)]}
    {'labels': []}
  }


  a -> b -> c
  |         |
  +---------+

  in the case of branch (br) instructions, the first edge is true
  and second edge is false
  """
  cfg = {'nodes': None, 'edges': [], 'labels': []}

  for func in program['functions']:
    cfg['nodes'] = blocks_form(func['instrs'])

    # precompute labels so we dont need to search when constructing cfg
    for node in cfg['nodes']:
      if node and 'label' in node[0].keys():
        cfg['labels'].append(node[0]['label'])

    for block in cfg['nodes']:
      # handle single successor, shit messy code fuck fuck fuck
      if block[-1]['op'] in TERM:
        if block[-1]['op'] == 'ret':
          cfg['edges'].append(block)
        elif block[-1]['op'] == 'call':
          cfg['edges'].append([block, block[-1]['funcs']])
        else:
          for label in block[-1]['labels']:
            if label in cfg['labels']:
              # IR only allows branch to labels
              cfg['edges'].append([block, label])
            else:
              print("Branch to invalid label: %s" % label)
  return cfg

if __name__ == '__main__':
  cfg = cfg_construct(load(stdin))
  print(dumps(cfg, indent=2))
