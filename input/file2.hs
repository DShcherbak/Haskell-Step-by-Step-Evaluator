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
testBadWhere x = go x where go 1 = 1
                            go n = n * go (n-1)

testSecondWhere 0 = 0
testSecondWhere n = helper (n^2)
                where
                    helper n | mod n 2 == 0 = 1
                             | otherwise    = 17

--elementAt :: [a] -> Int -> a