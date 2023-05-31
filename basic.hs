{-
  basic haskell
  resources:
  http://learnyouahaskell.com
  https://en.wikibooks.org/wiki/Haskell/
  https://wiki.haskell.org/

  playing with basic syntax, semantics and types within the language
  reference:
  head - returns the first element within a list
  last - returns the last element within a list
  tail - returns a list with the first element removed from the list
  init - returns a list with the last element removed from the list
  length - returns length of a list
  null - checks if a list empty and returns a boolean
  reverse - returns a reversed list
  take - takes an integer and a list, and removes the n elements from the front of the list
  drop - takes an integer and a list, and removes the n elements from the back of the list
  minimum - returns the element within the list with the lowest value
  maximum - returns the element within the list with the highest value
  sum - returns the sum of all the elements within the list
  product - returns the product of all elements within the list
  elem - checks if an element exists within a list, returns a boolean
  cycle - takes a list and repeats it infinitely, cut off with "take" function
  repeat - takes an element and repeats it infinitely, cut off with "take" function
  zip - produces a list of pairs from two lists
-}

-- basic types & Prelude functions
integer = 2 :: Integer
string = "hello" :: String
-- list
list = [1, 2, 3]
list' = [4, 5, 6]
-- [1..6]
-- the .. operator is also a function which returns a list between the range provided
list_concat = list++list'
-- we can place a value at the front of a list with the ':' cons operator
-- this is important for large lists, as haskell will not optimize for us
list_concat' = 5:list_concat
-- indexing lists is done with the !! function
list_first :: [a] -> a
list_first a = a !! 0
-- list comprehension
even_numbers = [x * 2 | x <- [1..10]]

-- yea i know im weird..
waifus = ["emilia", "megumin", "rem", "mai", "hayasaka"]
honorifics = ["sama", "san", "chan", "tan"]
-- im losing it..
best_girls = [waifu++" "++honorific | honorific <- honorifics, waifu <- waifus]
bg_index = zip [0..] waifus

-- tuple
tuple = ("hello", "there")

-- basic functions
fib :: Integer -> Integer
fib 0 = 0
fib 1 = 1
fib n = fib (n - 1) + fib (n - 2)
-- better functions
factorial :: Integer -> Integer
factorial n = product [1..n]

-- function gcd is already defined within Prelude module, so i call gcf :)
-- gcf(int x, int y) -> int {}
gcf :: Integer -> Integer -> Integer
gcf x 0 = x
gcf x y = gcf y (mod x y)

-- lambda abstractions
add :: Integer -> Integer -> Integer
add x y = x + y
-- add prime: curried LC only allows the application of one argument to an abstraction
add' = \x -> \y -> x + y
-- partial application of add
-- the arrow operator is right associative, which means add technically can recieve one argument and
-- return a function which returns an integer
-- Integer -> (Integer -> Integer)
-- functions are not partial, but the you may partially apply them
add_with_31 = add 31
add_result = add_with_31 19 -- should be 50

-- conditionals
is_zero :: Integer -> Bool
is_zero x = if x == 0 then True else False

-- polymorphic types within haskell
-- parametric polymorphism
-- this applies to functions which can work with many types.
foo :: a -> Int
foo c = 10

-- error function
-- useful for debugging various functions. fp is elegant and minimal, but complex simplicity
-- often leads to syntactical/semantic errors ;-;
-- there are other means error handling and propigation, such as exceptions; included within
-- the Control.exception module. i will only be using Prelude within basic.hs, which means
-- we are going to cover only "error" function.
head' :: [a] -> a
head'[] = error "head': empty list"
head' (x:_) = x

-- main function
main :: IO()
main = do
  print integer
  print string
  print list_concat'
  print even_numbers
  print best_girls
  print (fib 12)
  print (factorial 5)
  print (gcf 40 250)
  print ((add' 3)5)
  print add_result
  print (is_zero 1)
  print (foo "random type")
  print (head' [1..100])
