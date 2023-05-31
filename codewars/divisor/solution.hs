
divisors :: Integral a => a -> Int
divisors n = length [x | x <- [1..n], mod n x == 0]

-- debug
main = do
  print (divisors 1)
