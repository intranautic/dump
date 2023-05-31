module Main where

factorial :: Integer -> Integer
factorial n = facaux n 1 where
  facaux 0 a = a
  facaux n a = facaux (n - 1) (a * n)

{-
  fib(0) = 1
  fib(1) = 1
  fib(n + 2) = fib(n) + fib(n + 1), if n + 2 > 1
-}

fib :: Integer -> Integer
fib 0 = 1
fib 1 = 1
fib x = fib' 1 0 x where
  fib' a b 0 = a + b
  fib' a b n = fib' b (a + b) (n - 1)

main :: IO ()
main = undefined

