module Main where
import Data.Tuple (swap)
import Data.List (insertBy)

myLast :: [a] -> a
myLast (x:[]) = x
myLast (x:xs) =
    myLast xs

myButLast :: [a] -> a
myButLast (x:y:[]) = x
myButLast (x:xs) = myButLast xs

testBadWhere 0 = 0
testBadWhere x = go where go 1 = 1
                          go n = n * go (n-1)

--elementAt :: [a] -> Int -> a