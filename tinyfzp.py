"""
tiny fuzzy parser (tinyfzp)
context free grammar for tiny

BTW i dont write much python so style is gonna be fucked dont mind it
"""
from sys import argv

class LexerError(Exception):
  def __init__(self, token: str, message: str='Lexer error while tokenizing'):
    super().__init__(message + ': ' + token)
    self.token = token

# TODO: work on err information for parser
class ParserError(Exception):
  pass

"""
generic lexer class, should be modular and extensible
we can inherit from generic lexer and write a recursive
descent parser for the fuzzy context-free gramar (K-grammar)
"""
class generic_lexer:
  def __init__(self, source : str):
    self.source = source
    self.index : int = 0 # this is valid syntax??
 
  """ helper methods for lexer class """
  # return current index of lexer
  def tell(self) -> int:
    return self.index

  # return remaining characters left
  def tell_remaining(self) -> int:
    return len(self.source) - self.tell()

  # return input source
  def input(self) -> str:
    return self.source

  # return remaining source left to tokenize
  def remaining(self) -> str:
    return self.source[self.tell():]

  # is end of file?
  def is_eof(self) -> bool:
    return self.tell() >= len(self.source)

  def peek(self, offset=0) -> str:
    return 


"""
backus naur form (janky) for K-grammar of tiny
Program → stmt-sequence
stmt-sequence → statement { { ; } statement }
statement → if-stmt | repeat-stmt | read-stmt |write-stmt | fzy-stmt | assign- stmt
if- stmt → if A
A → exp B stmt-sequence [C stmt-sequence ] D
B → then | fuzzy_then
C → else | fuzzy_else
D → end | fuzzy_end
repeat- stmt → repeat rpt
rpt → stmt-sequence E exp
E → until | funtil
read- stmt → read identifier
write- stmt → write exp
fzy-stmt → fuzzy_read G | fuzzy_write F | fuzzy_if T | fuzzy_repeat H
G → fasg | identifier
fasg → read | write | L exp
L → = | = = | :=
F → fasg | exp
T → fasg | A
H → fasg | rpt
assign- stmt → identifier fasg
exp → simple-exp [comparison-op simple-exp]
comparison-op → < | L
simple-exp → term {addop term }
addop → + | -
term → factor{ mulop factor }
mulop → * | /
factor → ( exp K | number | rd
K → ) | ]
"""

class Ast:
  pass

class tiny_parser(generic_lexer):
  def __init__(self, buffer):
    super(tiny_parser, self).__init__(buffer)
    self.buffer = buffer

  def run(self) -> Ast:
    pass

if __name__ == '__main__':
  try:
    ast = tiny_parser(open(argv[1]).read()).run()

  except IndexError:
    print('Usage: %s <path/to/tiny>' % argv[0])

