{-# LANGUAGE RankNTypes #-}

import Prelude hiding (succ)

-- 
newtype Number = Nr (forall a. (a -> a) -> a -> a)

zero :: Number
zero = Nr (\ _ z -> z)

succ :: Number -> Number
succ (Nr a) = Nr (\ s z -> s (a s z))

one :: Number
one = succ zero

add :: Number -> Number -> Number
add (Nr a) = undefined

mult :: Number -> Number -> Number
mult (Nr a) b = undefined

pow :: Number -> Number -> Number
pow x (Nr n) = undefined

-- debug
--main = do
--  print ()
