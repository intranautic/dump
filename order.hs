-- playing with higher order functions, closures, and currying
-- the term higher order function simply refers to functions which operate on functions.
module Main where

-- higher order function which calls a function twice
twice :: (a -> a) -> a -> a
twice f x = f (f x)
-- map higher order function usage
-- filter higher order function usage
filter' p xs = [x | x <- xs, p x]


-- exercise: implement a for loop function
for :: a -> (a -> Bool) -> (a -> a) -> (a -> IO ()) -> IO ()
for init cond step job = undefined

-- composition operator function: '.'
-- application operator function: '$'
-- uncurry and curry
-- id and const

main :: IO ()
main = undefined
