-- beautiful 
import Data.List (partition)

qsort []      = []
qsort [a]     = [a]
qsort (a:as)  = let (lt,gt) = partition (<a) as
  in qsort lt ++ [a] ++ qsort gt
        
