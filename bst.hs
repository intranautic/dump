module Main where

data Tree a
  = Node a (Tree a) (Tree a)
  | Leaf a
  | Empty
    deriving (Eq, Show)

instance Functor Node where
  -- fmap :: (a -> b) -> Node a -> Node b
  fmap f (Node x l r) = Node (f x) (fmap f l) (fmap f r)
  fmap f (Leaf x) = Leaf (f x)
  fmap _ Empty = Empty

main :: IO ()
main = undefined

