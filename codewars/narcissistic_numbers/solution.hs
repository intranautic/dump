dig :: Integer -> [Integer]
dig 0 = []
dig n = dig(div n 10) ++ [mod n 10]
isNarcissistic :: Integer -> Bool
isNarcissistic n = do
  let digits = dig n
  if sum [x ^ length(digits) | x <- digits] == n then True else False

-- debugging
main = do
  print (sum [x ^ 3 | x <- (dig 9474)])
  print (isNarcissistic 153) -- true
  print (isNarcissistic 143) -- false
  print (isNarcissistic 9474)
