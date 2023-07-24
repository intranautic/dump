module Main where

data Expr
  = Value Int
  | Div Expr Expr

eval :: Expr -> Maybe Int

safediv :: Int -> Int -> Maybe Int
safediv _ 0 = Nothing
safediv x y = Just (x `div` y)

-- inefficient safe div by currying with functors
-- we can use applicatives to shorten this code.
eval (Value x) = Just x
eval (Div l r) =
  case eval l of
    Nothing -> Nothing
    Just n -> case eval r of
        Nothing -> Nothing
        Just m -> safediv n m

-- evaluation implemented via monadic binds
-- we can make it even simpler with the do notation for extra sugar
-- (>>=) :: Maybe a -> (a -> Maybe b) -> Maybe b
eval (Value x) = pure x
eval (Div l y) =
  eval l >>= \n ->
  eval y >>= \m -> 
    safediv n m


eval (Value x) = Just x
eval (Div l r) = 
  do  n <- eval l
      m <- eval r
      safediv n m


main :: IO ()
main = undefined

