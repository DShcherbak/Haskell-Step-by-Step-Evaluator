good'N't' ('a':('b':"c   sdf (")) = "abc"
good'N't' ('1':('2':['3','4','5'])) = "12345"
good'N't' ['3'] = "--3--"
good'N't' x = "---"

hardComment "{-" = "{-" {- -- hello a {- tricccccky -} comment here -}
            ++ "{-"

complexWhere' :: Num a => a -> a -> a -> a
complexWhere' x y z = let{a = t1 + t2 where{t1 = x ;t2 = y}} in let{b = t3 + a where{t3 = z}} in b ^ q where{q = b2 - b1 - kk where{b1 = 1; b2 = 2; kk = 1} ;kk = 0}

doWithWhere :: IO ()
doWithWhere = do
    let {x = 2} 
        in print x
    let x = let 
            p = 4 in p

    let {y = 4}
    print (x+y+z)
        where
            z = q
            q = r
            r = 1



letConfused = let p = 3 
    in p * p

let1 x = let a = b + 1
             b = x + 1
             in let d = a + 1
                    e = d + 1
                        in e

complexWhere :: Num a => a -> a -> a -> a
complexWhere x y z = let a = t1 + t2 where t1 = x
                                           t2 = y
                        in let b = t3 + a where t3 = z
                            in b ^ q where q = b2 - b1 - kk where b1 = 1; b2 = 2; kk = 1
                                           kk = 0
{--
complexWhere x y z = let {a = t1 + t2 where{t1 = x;t2 = y}in let{b = t3 + a where{t3 = z}}in b ^ q where{q = b2 - b1 - kk where{b1 = 1; b2 = 2; kk = 1};kk = 0}
                                           -}

anotherTest x = let p = (x^2)in(+)p x 

mixLetAndWhere x = let p = y where y = x + 1 in p

closedAndUnclosedLet x = let {p = 3 ; d = let q = x in d} in p + d  

complexLNW= let p = q where q = let{z = 1}in z in p 

complexLNW' = let{p = q where{q = let{z = 1}in z }}in p 



doubleWhere :: Integral b => b -> b -> (b, Bool)
doubleWhere a b = (x, t) where
    x = y + 1 where
        y = a ^ b
    t = qq > 0 where
        qq = a - b


myLast :: [a] -> a
myLast [] = error "Error"
myLast [x] = x
myLast (x:xs) =
    myLast xs

myButLast :: [a] -> a
myButLast [] = error "Error"
myButLast [x] = error "Error"
myButLast [x, y] = x
myButLast (x:xs) = myButLast xs

data Number1 = Int :+ Int
data Number2 = Int :$ Int
data Number2D = Double :$$ Double
data Number3 = Number2 :$$$ Number2

data SC = SomeCust Int Int Int | SomeCust' Int Int [Int]


goodForNothing (1 :+ m, (('a', "(b,c)"),             ["d,e", _, n,
            '(' : 'f' : ",g)"], "( asd)" )) = "PARSED!"
goodForNothing _ = "doesn't parse..."

goodForAll ((:+) 1 2) (3 :+ 4) = 10
goodForAll _ _ = 9

goodForAll2 ((:$) 1 2) (3 :$ 4) = 9
goodForAll2 _ _ = 10

goodForAll3 :: Either Number2D Number3 -> Int
goodForAll3 (Right (((:$) 3 4) :$$$ (1 :$ 2))) = 10
goodForAll3 (Left ((:$$) 2.3 412345678009)) = 10
goodForAll3 _ = 0

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