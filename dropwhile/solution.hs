import Prelude hiding (dropWhile, span, break, filter)

dropWhile :: [a] -> (a -> Bool) -> [a]
--dropWhile xs p = [x | x <- xs, p x == True]
dropWhile xs p = if null xs then [] else if p (xs !! 0) == True then dropWhile (tail xs) p else xs
-- debug
main = do
  print (dropWhile [] even)
  print (dropWhile [1..10] even)
