balancedParens :: Int -> [String]
balancedParens 0 = [""]
balancedParens n = []

-- debug
main = do
  print (balancedParens 0)
  print (balancedParens 1)
  print (balancedParens 2)
  print (balancedParens 3)
