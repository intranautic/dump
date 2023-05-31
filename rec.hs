-- recursion exercises
import Prelude hiding ((!!))
and' :: [Bool] -> Bool
and' [] = True
and' (b:bs) = b && and' bs
concat' :: [[a]] -> [a]
concat' [] = []
concat' (xs:xss) = xs ++ concat' xss
replicate' :: Int -> a -> [a]
replicate' 0 x = []
replicate' n x = x : replicate' (n-1) x
(!!) :: [a] -> Int -> a
(x:_) !! 0 = x
(_:xs) !! n = xs !! (n - 1)
-- sorting algorithms
insert' :: (Ord a) => a -> [a] -> [a]
insert' x [] = [x]
insert' x (y:ys) =
  if x <= y
    then x:y:ys
    else y : insert' x ys
insertionsort :: (Ord a) => [a] -> [a]
insertionsort [] = []
insertionsort (x:xs) = insert' x (insertionsort xs)

merge :: (Ord a) => [a] -> [a] -> [a] 
merge [] ys = ys
merge xs [] = xs
merge (x:xs) (y:ys) =
  if x <= y
    then x : merge xs (y:ys)
    else y : merge (x:xs) ys
halve :: (Ord a) => [a] -> ([a], [a])
halve [] = ([], [])
halve xs = (take ((length xs) `div` 2) xs, drop ((length xs) `div` 2) xs)
mergesort :: (Ord a) => [a] -> [a]
mergesort [] = []
mergesort [x] = [x]
mergesort xs = merge (mergesort ys) (mergesort zs) where
  (ys, zs) = halve xs

quicksort :: (Ord a) => [a] -> [a]
quicksort [] = []
quicksort (x:xs) =
  low ++ [x] ++ high where
    low = quicksort [n | n <- xs, n <= x]
    high = quicksort [n | n <- xs, n > x]

-- tail recursion


main = do
  print (and' [True, True, False])
  print (concat' [[1, 45, 1], [5, 6, 7]])
  print (replicate' 10 'a')
  print ([2] !! 0)
  print (insertionsort [5, 3, 1, 100, 3, 2, 4, 6, 123])
  print (mergesort [5, 3, 1, 100, 3, 2, 4, 6, 123])
  print (quicksort [5, 3, 1, 100, 3, 2, 4, 6, 123])
