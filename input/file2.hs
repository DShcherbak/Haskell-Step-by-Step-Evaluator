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

--elementAt :: [a] -> Int -> a