module Main where
import Data.List

type Grid = Matrix Valve
type Matrix a = [Row a]
type Row a = [a]
type Valve = Char

empty :: Grid
empty = replicate 9 (replicate 9 '.')

-- identity function for rows, nice utility to have
rows :: Matrix a -> [Row a]
rows = id

-- id = cols . cols
cols :: Matrix a -> [Row a]
cols = transpose

boxes :: Matrix a -> [Row a]
boxes = undefined



main :: IO ()
main = undefined

