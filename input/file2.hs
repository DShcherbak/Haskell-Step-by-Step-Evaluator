--module Main where
--import Data.Tuple (swap)
--import Data.List (insertBy)

--myLast :: [a] -> a
--myLast (x:[]) = x
--myLast (x:xs) =
--    myLast xs

--myButLast :: [a] -> a
--myButLast (x:y:[]) = x
--myButLast (x:xs) = myButLast xs

--data Number1 = Int :+ Int

--goodForNothing (1 :+ m, (('a', "(b,c)"),             ["d,e", _, n,
--            '(' : 'f' : ",g)"], "( asd)" )) = "PARSED!"
--goodForNothing _ = "doesn't parse..."

--good'N't' ('a':('b':"c   sdf (")) = "abc"
good'N't' (1:(2:[3,4,5])) = "12345"
good'N't' [3] = "--3--"
good'N't' x = "---"

goodForAll ((:+) 1 2) (3 :+ 4) = 10
goodForAll _ _ = 9

parsingNames (Left errorMsg) = Left ("Got error: " ++ errorMsg)
parsingNames (Right (Just x)) = Right $ "Got result: " ++ show x
parsingNames _ = Left "Something weird"

parsingCusNamed (SomeCust 1 2 3) = 1
parsingCusNamed (SomeCust' _ x [1,2,3]) = 2
parsingCusNamed _ = 3



guarded n | n > 0     = "Big"
          | n == 0    = "Medium"
          | n /= (-1) = "XMedium"
          | otherwise = "Small"

testBadWhere 0 = 0
testBadWhere x = go x where go 1 = 1
                            go n = n * go (n-1)

testSecondWhere 0 = 0
testSecondWhere n = helper (n^2)
                where
                    helper n | mod n 2 == 0 = 1
                             | otherwise    = 17

--elementAt :: [a] -> Int -> a