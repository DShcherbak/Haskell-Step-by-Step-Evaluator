module Main where
import Data.Tuple (swap)
import Data.List (insertBy)

myLast :: [a] -> a
myLast (x:[]) = x
myLast (x:xs) = myLast xs

myButLast :: [a] -> a
myButLast (x:y:[]) = x
myButLast (x:xs) = myButLast xs

elementAt :: [a] -> Int -> a
elementAt (x:[]) _ = x
elementAt (x:_) 1 = x
elementAt (x:xs) n = elementAt xs (n - 1)

myLength :: [a] -> Int
myLength [] = 0
myLength (_:xs) = 1 + myLength xs

myReverse :: [a] -> [a]
myReverse lst = helper [] lst where
    helper xs [] = xs
    helper xs (y:ys) = helper (y:xs) ys


isPalindrome :: (Eq a) => [a] -> Bool
isPalindrome lst = lst == (myReverse lst)

data NestedList a = Elem a | List [NestedList a]

flatten :: (Eq a) => NestedList a -> [a]
flatten (Elem a) = [a]
flatten (List xs) = concat $ map flatten xs


compress :: [Char] -> [Char]
compress (x : []) = x : []
compress (x : y : xs) = if x /= y then x : tl else tl where tl = compress (y:xs)

pack :: (Eq a) => [a] -> [[a]]
pack lst = helper [] lst where
    helper [] [] = [[]]
    helper [] (x:xs) = helper [x] xs
    helper (x:xs) [] = (x:xs) : []
    helper (x:xs) (y:ys) = if x == y then helper (y:x:xs) ys else ((x:xs) : helper [y] ys)


encode :: (Eq a) => [a] -> [(Int, a)]
encode lst = map (\(x:xs) -> (myLength (x:xs), x)) (pack lst)

data RepeatedValue a = Single a | Multiple Int a deriving Eq

encodeModified :: (Eq a) => [a] -> [RepeatedValue a]
encodeModified lst = map helper (encode lst) where
    helper (1,x) = Single x
    helper (n,x) = Multiple n x

decodeModified :: (Eq a) => [RepeatedValue a] -> [a]
decodeModified [] = []
decodeModified (Single x : xs) = x : decodeModified xs
decodeModified (Multiple n x : xs) = (repeat n x) ++ decodeModified xs where
    repeat 0 x = []
    repeat n x = (x : (repeat (n - 1) x))


encodeDirect :: (Eq a) => [a] -> [RepeatedValue a]
encodeDirect (x:xs) = helper 0 x (x:xs) where
    helper 0 _ [] = []
    helper 1 x [] = [(Single x)]
    helper n x [] = [(Multiple n x)]
    helper 0 _ (x:xs) = helper 1 x xs
    helper n x (y:ys) = if x == y then helper (n+1) x ys else (hd : (helper 1 y ys))
        where hd = if n == 1 then (Single x) else (Multiple n x)

dupli :: (Eq a) => [a] -> [a]
dupli [] = []
dupli (x:xs) = x:x:(dupli xs)


repli :: (Eq a) => [a] -> Int -> [a]
repli [] _ = []
repli (x:xs) n = head x n ++ (repli xs n) where
    head x 0 = []
    head x n = x : head x (n-1)

dropEvery :: (Eq a) => [a] -> Int -> [a]
dropEvery lst n = helper (n-1) lst (n-1) where
    helper _ [] _ = []
    helper n (x:xs) 0 = helper n xs n
    helper n (x:xs) m = x : helper n xs (m - 1)

split :: (Eq a) => [a] -> Int -> ([a], [a])
split lst n = (take n lst, drop n lst)

slice :: (Eq a) => [a] -> Int -> Int -> [a]
slice lst n m = take (m-n+1) $ drop (n-1) lst


rotate :: (Eq a) => [a] -> Int -> [a]
rotate lst n = drop r lst ++ take r lst where
    l = length lst
    r = helper n l
    helper n l | n >= 0 = n `mod` l
               | otherwise = helper (n + l) l

removeAt :: (Eq a) => Int -> [a] -> (a, [a])
removeAt n lst = (head $ drop (n-1) lst, take (n-1) lst ++ drop n lst)

insertAt :: (Eq a) => a -> [a] -> Int -> [a]
insertAt x lst 1 = x : lst
insertAt x [] n = x : []
insertAt x (y:ys) n = y : insertAt x ys (n - 1)

range :: Int -> Int -> [Int]
range n m = if n > m then [] else n : range (n+1) m

combinations :: (Eq a) => Int -> [a] -> [[a]]
combinations _ [] = [[]]
combinations 0 ls = [[]]
combinations n (x:xs) = x_start ++ others where
    x_start = [x:ys | ys <- combinations (n-1) xs]
    others = if n <= length xs then [zs | zs <- combinations n xs] else []

group :: (Eq a) => [Int] -> [a] -> [[[a]]]
group [] _ = [[]]
group _ [] = [[]]
group (n:ns) (e:es) = [x:xs | x <- (combinations n (e:es)), xs <- group ns (lefts x (e:es))] where
    contains [] e = False
    contains (t:ts) e = if t == e then True else contains ts e
    lefts _ [] = []
    lefts [] ls = ls
    lefts (e:excludes) (x:elements) = if x == e then lefts excludes elements else x : (lefts (e:excludes) elements)



mergeSort :: (Ord a, Eq a) => [a] -> (a -> a -> Ordering) -> [a]
mergeSort [] comp = []
mergeSort (x:[]) comp = (x:[])
mergeSort lst comp = mergeHalfes left right where
    len = div (length lst) 2
    left = mergeSort (take len lst) comp
    right = mergeSort (drop len lst) comp
    merHalfes l [] = l
    mergeHalfes [] r = r
    mergeHalfes (l:ls) (r:rs) = if comp l r < GT then (l : mergeHalfes ls (r:rs)) else (r : mergeHalfes rs (l:ls))

lsort :: (Ord a, Eq a) => [[a]] -> [[a]]
lsort lst = mergeSort lst lenCompare where
    lenCompare a b = let res = compare (length a) (length b)
                        in if res == EQ then compare a b else res


lfsort :: Ord a => [[a]] -> [[a]] --Why Eq a here causes an error??
lfsort lst = concat . ungroup $ mergeSort grouped tupleComp where
    tupleComp (a, xs) (b, ys) =let res = compare a b
                        in if res == EQ then compare xs ys else res
    ungroup [] = []
    ungroup ((a, lst) : xs) = lst : ungroup xs
    grouped = helperGrouped [] (lsort lst)
    helperGrouped [] [] = []
    helperGrouped lst [] = (length lst, lst) : []
    helperGrouped [] (x:xs) = helperGrouped [x] xs
    helperGrouped (y:ys) (x:xs) = if length x == length y then helperGrouped ((y:ys) ++ [x]) xs else (1 + length ys, (y:ys)) : helperGrouped [x] xs


isPrime :: (Integral a) => a -> Bool
isPrime p = primer p $ map (\x -> (x, True)) [2..(div p 2)] where
    primer p [] = True
    primer p ((x, xp) :xs) = if not xp then primer p xs else
        if mod p x == 0 then False else primer p $ filterPrimes xs (x-1) (x-1)
    filterPrimes [] pr _ = []
    filterPrimes ((cur, curPrime):ys) pr 0 = (cur,False) : filterPrimes ys pr pr
    filterPrimes (y:ys) pr cnt = y : filterPrimes ys pr (cnt - 1)


myGcd :: (Integral a) => a -> a -> a
myGcd a b = let (d,x,y) = ext a b in abs d
    where ext a b = if b == 0 then (a,1,0) else (d1, y1, x1-(div a b) * y1) where (d1, x1, y1) = ext b (mod a b)

coprimes :: Integral a => a -> a -> Bool
coprimes a b = (myGcd a b) == 1


totient :: Int -> Int
totient a = length $ filter (coprimes a) ([1..a])

primeFactors :: Int -> [Int]
primeFactors p = primer p $ map (\x -> (x, True)) [2..(div p 2)] where
    primer p [] = [p]
    primer p prs@((x, xp) :xs) = if not xp then primer p xs else
        if mod p x == 0 then let new_p = (div p x) in x : primer new_p (takeWhile (\(x,_) -> x * x <= new_p) prs) else primer p $ filterPrimes xs (x-1) (x-1)
    filterPrimes [] pr _ = []
    filterPrimes ((cur, curPrime):ys) pr 0 = (cur,False) : filterPrimes ys pr pr
    filterPrimes (y:ys) pr cnt = y : filterPrimes ys pr (cnt - 1)

primeFactorsMult :: Int -> [(Int, Int)]
primeFactorsMult = (map swap) . encode . primeFactors

primeFactorsSuper :: Int -> String
primeFactorsSuper = (concatStringsWith " * ") . (map (\(a,b) -> (show a ++ "^" ++ show b))) . primeFactorsMult


concatStringsWith :: [Char] -> [[Char]] -> [Char]
concatStringsWith sep (x:y:xs) = x ++ sep ++ concatStringsWith sep (y:xs)
concatStringsWith _ (x:[]) = x
concatStringsWith _ [] = ""


totient2 :: Int -> Int
totient2 = (foldr (*) 1) . (map (\(a,b) -> (a-1) * (a ^ (b-1)))) . primeFactorsMult

primeRange :: Int -> Int -> [Int]
primeRange left right = dropWhile (\x -> x < left) $ primer $ map (\x -> (x, True)) [2..right] where
    primer [] = []
    primer prs@((x, xp) :xs) = if not xp then primer xs else x : (primer $ filterPrimes xs (x-1) (x-1))
    filterPrimes [] pr _ = []
    filterPrimes ((cur, curPrime):ys) pr 0 = (cur,False) : filterPrimes ys pr pr
    filterPrimes (y:ys) pr cnt = y : filterPrimes ys pr (cnt - 1)


goldbach :: Int -> (Int, Int)
goldbach n = head [(x,y) | x <- rng, y <- rng, x + y == n] where rng = primeRange 2 n

goldbachList :: Int -> Int -> [(Int, Int)]
goldbachList l r = map goldbach [q,(q+2)..r] where q = if mod l 2 == 0 then l else l + 1


goldbachList' :: Int -> Int -> Int -> [(Int, Int)]
goldbachList' l r u = filter (\(a,b) -> a > u && b > u) $ map goldbach [q,(q+2)..r] where q = if mod l 2 == 0 then l else l + 1


not' :: Bool -> Bool
not' True = False
not' _ = True

and' :: Bool -> Bool -> Bool
and' True True = True
and' _ _ = False

or' :: Bool -> Bool -> Bool
or' False False = False
or' _ _ = True

nand' :: Bool -> Bool -> Bool
nand' x y = not' (and' x y)

nor' :: Bool -> Bool -> Bool
nor' x y = not' (or' x y)

xor' :: Bool -> Bool -> Bool
xor' x y = not' $ equ' x y

impl' :: Bool -> Bool -> Bool
impl' x y =  (not' y) `or'` (and' x y)

equ' :: Bool -> Bool -> Bool
equ' x y = (and' x y) `or'` (nor' x y)

table :: (Bool -> Bool -> Bool) -> IO()
table f = putStrLn $ concat $ map (\(a,b,c) -> show a ++ " " ++ show b ++ " " ++ show c ++ "\n")  [(x,y,f x y) | x <- tf, y <- tf] where tf = [True, False]

tablen :: Int -> ([Bool] -> Bool) -> IO()
tablen n func = putStrLn $ concat $ map (\(a, b) -> (concatStringsWith " " (map show a) ) ++ " " ++ show b ++ "\n")  [(inp, (func inp)) | inp <- inputs n]  where
    inputs 0 = [[]]
    inputs n = [x : xs | x <- [True, False], xs <- inputs (n-1)]

gray :: Int -> [String]
gray 0 = [[]]
gray n = [x:xs | x <- ['0','1'], xs <- gray (n-1)]

gray1 :: Int -> [String]
gray1 0 = [""]
gray1 n = foldr (\s acc -> ("0"++s) : ("1"++s) : acc) [] $ gray1 (n - 1)


data HufTree a = HufLeaf a | HufBranch (HufTree a) (HufTree a) deriving Show

getHuffmanCodes :: (Ord a1, Ord a2, Num a2) => [(a1, a2)] -> [(a1, [Char])]
getHuffmanCodes  = (decode "") . buildHufTree where
    decode pref (HufBranch left right) = (decode ('0':pref) left) ++ (decode ('1':pref) right)
    decode pref (HufLeaf x) = [(x, reverse pref)]

buildHufTree :: (Ord a1, Ord a2, Num a2) => [(a1, a2)] -> HufTree a1
buildHufTree list = snd . mergeHufTree $ map (\(a,b) -> (b, HufLeaf a)) $ mergeSort list (\(a,b) (c,d) -> compare b d) where
        mergeHufTree ((wg, leaf) : []) = (wg, leaf)
        mergeHufTree ((w1, t1):(w2, t2):rest) = mergeHufTree $ insertBy (\(a,b) (c,d) -> compare a c) (w1+w2, HufBranch t1 t2) rest

data Tree a = Empty | Branch a (Tree a) (Tree a)
              deriving (Show, Eq)

leaf :: a -> Tree a
leaf a = Branch a Empty Empty

allBinaryTrees :: Int -> [Tree Char]
allBinaryTrees n | n == 0       = [Empty]
                 | n < 0        = []
                 | mod n 2 == 0 = let (k,m) = (div n 2, n - k - 1) in
                    [(Branch 'x' t1 t2) | t1 <- allBinaryTrees k, t2 <- allBinaryTrees m] ++
                    [(Branch 'x' t2 t1) | t1 <- allBinaryTrees k, t2 <- allBinaryTrees m]
                 | otherwise    = let m = (div (n - 1) 2) in
                        [Branch 'x' t1 t2 | t1 <- allBinaryTrees m, t2 <- allBinaryTrees m] ++
                        [Branch 'x' t1 t2 | t1 <- allBinaryTrees (m-1), t2 <-allBinaryTrees (m+1),
                            abs ((treeHeight t1) - (treeHeight t2)) < 2]
                                where
                                    treeHeight Empty = 0
                                    treeHeight (Branch _ t1 t2) = 1 + max (treeHeight t1) (treeHeight t2)


symmetric :: (Eq a) => Tree a -> Bool
symmetric Empty = True
symmetric (Branch _ t1 t2) = isMirror t1 t2 where
    isMirror Empty Empty = True
    isMirror (Branch x t11 t12) (Branch y t21 t22) = isMirror t11 t22 && isMirror t12 t21
    isMirror _ _ = False


construct :: (Eq a, Ord a) => [a] -> Tree a
construct [] = Empty
construct (x:xs) = helper (Branch x Empty Empty) xs where
    helper tree [] = tree
    helper tree (x:xs) = helper (add x tree) xs
    add x Empty = Branch x Empty Empty
    add x (Branch v left right) = case compare x v of
        LT -> Branch v (add x left) right
        EQ -> Branch v left right
        GT -> Branch v left (add x right)

symCbalTrees :: Int -> [Tree Char]
symCbalTrees n  | n == 0                = [Empty]
                | n < 0 || mod n 2 == 0 = []
                | otherwise             = [Branch 'x' t1 (reverse t1) | t1 <- allBinaryTrees (div (n-1) 2)] where
                    reverse Empty = Empty
                    reverse (Branch x left right) = Branch x (reverse right) (reverse left)


hbalTree :: (Eq a, Ord a) => a -> Int -> [Tree a]
hbalTree e 0 = [Empty]
hbalTree e 1 = [Branch e Empty Empty]
hbalTree e h = [Branch e left right |
            (left_heigth, right_height) <- [(h-2, h-1), (h-1,h-2), (h-1, h-1)],
            left <- hbalTree e left_heigth, right <- hbalTree e right_height]

hbalTreeWithCount :: (Eq a, Ord a) => a -> Int -> [(Tree a, Int)]
hbalTreeWithCount e 0 = [(Empty, 0)]
hbalTreeWithCount e 1 = [(Branch e Empty Empty, 1)]
hbalTreeWithCount e h = [(Branch e left right, lc + rc + 1) |
            (left_heigth, right_height) <- [(h-2, h-1), (h-1,h-2), (h-1, h-1)],
            (left, lc) <- hbalTreeWithCount e left_heigth, (right, rc) <- hbalTreeWithCount e right_height]

fibs :: [Int]
fibs = 0 : 1 : zipWith (+) fibs (tail fibs)

hbalTreeByNodes :: (Eq a, Ord a) => a -> Int -> [Tree a]
hbalTreeByNodes e 0 = [Empty]
hbalTreeByNodes e 1 = [Branch e Empty Empty]
hbalTreeByNodes e n = [Branch e left right |
            (left, lc, lh) <- possible_trees,
            (right, rc, rh) <- possible_trees,
            n == lc + rc + 1,
            abs (lh - rh) <= 1] where
                possible_trees = concat [map (\(x,y) -> (x,y,h)) (hbalTreeWithCount e h) | h <- [minH - 2 .. maxH - 1]]
                minH = ceiling $ logBase 2 $ fromIntegral (n+1)
                maxH = length (takeWhile (<= n+1) fibs) - 3


hbalTreeNodes' :: (Eq a, Ord a) => a -> Int -> [Tree a]
hbalTreeNodes' _ 0 = [Empty]
hbalTreeNodes' x n = concatMap toFilteredTrees [minHeight .. maxHeight]
    where toFilteredTrees = filter ((n ==) . countNodes) . hbalTree x

          -- Similar to the Fibonacci sequence but adds 1 in each step.
          minNodesSeq = 0:1:zipWith ((+).(1+)) minNodesSeq (tail minNodesSeq)
          minNodes = (minNodesSeq !!)

          minHeight = ceiling $ logBase 2 $ fromIntegral (n+1)
          maxHeight = length (takeWhile (<= n+1) fibs) - 3

          countNodes Empty = 0
          countNodes (Branch _ l r) = countNodes l + countNodes r + 1


countLeaves :: (Eq a, Ord a) => Tree a -> Int
countLeaves Empty = 0
countLeaves (Branch _ Empty Empty) = 1
countLeaves (Branch _ left right) = countLeaves left + countLeaves right

leaves :: (Eq a, Ord a) => Tree a -> [a]
leaves Empty = []
leaves (Branch x Empty Empty) = [x]
leaves (Branch _ left right) = leaves left ++ leaves right

internals :: (Eq a, Ord a) => Tree a -> [a]
internals Empty = []
internals (Branch x Empty Empty) = []
internals (Branch y left right) = y : (internals left ++ internals right)

atLevel :: (Eq a, Ord a) => Int -> Tree a -> [a]
atLevel _ Empty = []
atLevel n (Branch x left right) | n < 0     = []
                                | n == 0    = [x]
                                | otherwise = (atLevel (n-1) left) ++ (atLevel (n-1) right)

tree4 :: Tree Integer
tree4 = Branch 1 (Branch 2 Empty (Branch 4 Empty Empty))
                 (Branch 3 Empty Empty)


completeBinaryTree :: (Eq a, Ord a) => a -> Int -> Tree a
completeBinaryTree e 0 = Empty
completeBinaryTree e 1 = leaf e
completeBinaryTree e n = Branch e left right where
    left  = completeBinaryTree e ln
    right = completeBinaryTree e rn
    (ln,rn) = let (offbase,maxBase) = (n - 1 - rightHalfBase, rightHalfBase * 2 + 1) in
        if offbase < (maxBase) then
            (if offbase > rightHalfBase then (offbase,rightHalfBase) else (rightHalfBase,offbase))
        else (maxBase,n-1-maxBase)
    rightHalfBase = snd $ lastThat (\(x,y) -> x > y) $ map ((\x -> ((n-1)-x, x)).(+(-1)).(2^)) [1..]
    lastThat f (x:[]) = x
    lastThat f (x:y:xs) = if not $ f y then x else lastThat f (y:xs)

isComplete :: (Eq a, Ord a) => Tree a -> Bool
isComplete Empty = True
isComplete (Branch _ left right) = isComplete left && isComplete right &&
                                    ((hl == hr) || (hl == hr + 1)) where
                                        hl = height left
                                        hr = height right
                                        height Empty = 0
                                        height (Branch _ t1 t2) = 1 + max (height t1) (height t2)

tree64 = Branch 'n'
                (Branch 'k'
                        (Branch 'c'
                                (Branch 'a' Empty Empty)
                                (Branch 'h'
                                        (Branch 'g'
                                                (Branch 'e' Empty Empty)
                                                Empty
                                        )
                                        Empty
                                )
                        )
                        (Branch 'm' Empty Empty)
                )
                (Branch 'u'
                        (Branch 'p'
                                Empty
                                (Branch 's'
                                        (Branch 'q' Empty Empty)
                                        Empty
                                )
                        )
                        Empty
                )

layout :: (Eq a, Ord a) => Tree a -> Tree (a, (Int, Int))
layout Empty = Empty
layout hd@(Branch x left right) = fst $ helper (-1) 0 hd where
    helper counter depth Empty = (Empty, counter)
    helper counter depth (Branch x left right) =
        let (new_left_tree, new_counter) = (helper counter (depth+1) left)
        in (
                let (new_right_tree, after_counter) = (helper (new_counter+1) (depth+1) right)
                in ((Branch (x, (new_counter + 1, depth)) new_left_tree new_right_tree), after_counter)
            )

height :: (Eq a, Ord a) => Tree a -> Int
height Empty = 0
height (Branch _ l r) = 1 + max (height l) (height r)

tree65 = Branch 'n'
                (Branch 'k'
                        (Branch 'c'
                                (Branch 'a' Empty Empty)
                                (Branch 'e'
                                        (Branch 'd' Empty Empty)
                                        (Branch 'g' Empty Empty)
                                )
                        )
                        (Branch 'm' Empty Empty)
                )
                (Branch 'u'
                        (Branch 'p'
                                Empty
                                (Branch 'q' Empty Empty)
                        )
                        Empty
                )

layout2 :: (Eq a, Ord a) => Tree a -> Tree (a, (Int, Int))
layout2 Empty = Empty
layout2 root@(Branch x left right) = fst $ helper (0) 1 root where
    max_depth = height root
    helper x_coord depth Empty = (Empty, x_coord)
    helper x_coord depth (Branch x left right) =
        let (new_left_tree, left_x) = (helper x_coord (depth+1) left)
        in (
                let (new_right_tree, _) = (helper (left_x + 2^(max_depth-depth)) (depth+1) right)
                in ((Branch (x, (left_x + 2^(max_depth-depth), depth-1)) new_left_tree new_right_tree), left_x)
            )

layout3 :: (Eq a, Ord a) => Tree a -> Tree (a, (Int, Int))
layout3 Empty = Empty
layout3 root@(Branch x left right) = fst $ helper (0) 1 root where
    max_depth = height root
    helper x_coord depth Empty = (Empty, x_coord)
    helper x_coord depth (Branch x left right) =
        let (new_left_tree, left_x) = (helper x_coord (depth+1) left)
        in (
                let (new_right_tree, _) = (helper (left_x + 2^(max_depth-depth)) (depth+1) right)
                in ((Branch (x, (left_x + 2^(max_depth-depth), depth-1)) new_left_tree new_right_tree), left_x)
            )



cbt = completeBinaryTree 0

tests :: [Bool]
tests = [myLast [1,2,3] == 3,
        myButLast [1..300] == 299,
        elementAt "haskell" 5 == 'e',
        myLength "Hello, world!" == 13,
        myReverse "A man, a plan, a canal, panama!" == "!amanap ,lanac a ,nalp a ,nam A",
        isPalindrome "madamimadam",
        not $ isPalindrome "madamimalam",
        (flatten (Elem 5)) == [5],
        (flatten (List [Elem 1, List [Elem 2, List [Elem 3, Elem 4], Elem 5]])) == [1,2,3,4,5],
        (flatten (List [])) == ([] :: [Int]),
        compress "aaaabccaadeeee" == "abcade",
        pack ['a', 'a', 'a', 'a', 'b', 'c', 'c', 'a',
                'a', 'd', 'e', 'e', 'e', 'e'] == ["aaaa","b","cc","aa","d","eeee"],
        encode "aaaabccaadeeee" == [(4,'a'),(1,'b'),(2,'c'),(2,'a'),(1,'d'),(4,'e')],
        (encodeModified "aaaabccaadeeee") == [Multiple 4 'a',Single 'b',Multiple 2 'c',
                                                Multiple 2 'a',Single 'd',Multiple 4 'e'],
        decodeModified [Multiple 4 'a',Single 'b',Multiple 2 'c',
            Multiple 2 'a',Single 'd',Multiple 4 'e'] == "aaaabccaadeeee",
        encodeDirect "aaaabccaadeeee" == [Multiple 4 'a',Single 'b',Multiple 2 'c', Multiple 2 'a',Single 'd',Multiple 4 'e'],
        dupli [1, 2, 3] == [1,1,2,2,3,3],
        repli "abc" 3 == "aaabbbccc",
        dropEvery "abcdefghik" 3 == "abdeghk",
        split "abcdefghik" 3 == ("abc", "defghik"),
        slice "abcdefghik" 3 7 == "cdefg",
        rotate ['a','b','c','d','e','f','g','h'] 3 == "defghabc",
        rotate ['a','b','c','d','e','f','g','h'] (-2) == "ghabcdef",
        removeAt 2 "abcd" == ('b',"acd"),
        insertAt 'X' "abcd" 2 == "aXbcd",
        range 4 9 == [4,5,6,7,8,9],
        length (combinations 3 "abcdef") == 20,
        length (combinations 3 "abcdefg") == 35,
        length (group [2,3,4] ["aldo","beat","carla","david","evi","flip","gary","hugo","ida"]) == 1260,
        length (group [2,2,5] ["aldo","beat","carla","david","evi","flip","gary","hugo","ida"]) == 756,
        lsort ["abc","de","fgh","de","ijkl","mn","o"] == ["o","de","de","mn","abc","fgh","ijkl"],
        lfsort ["abc", "de", "fgh", "de", "ijkl", "mn", "o"] == ["ijkl","o","abc","fgh","de","de","mn"],
        isPrime 7,
        not $ isPrime 2743,
        coprimes 35 64,
        [myGcd 36 63, myGcd (-3) (-6), myGcd (-3) 6] == [9,3,3],
        totient 10 == 4,
        primeFactors 315 == [3, 3, 5, 7],
        primeFactorsMult 2400 == [(2,5),(3,1),(5,2)],
        primeRange 10 20 == [11,13,17,19],
        goldbachList 9 20 == [(3,7),(5,7),(3,11),(3,13),(5,13),(3,17)],
        not $ symmetric (Branch 'x' (Branch 'x' Empty Empty) Empty),
        symmetric (Branch 'x' (Branch 'x' Empty Empty) (Branch 'x' Empty Empty)),
        symmetric . construct $ [3, 2, 5, 7, 1],
        symmetric . construct $ [5, 3, 18, 1, 4, 12, 21],
        length (symCbalTrees 5) == length [Branch 'x' (Branch 'x' Empty (Branch 'x' Empty Empty)) (Branch 'x' (Branch 'x' Empty Empty) Empty),Branch 'x' (Branch 'x' (Branch 'x' Empty Empty) Empty) (Branch 'x' Empty (Branch 'x' Empty Empty))],
        True
        ]
        --274346225891203207735274288647


firstWrong :: [Bool] -> Int
firstWrong xs = firstWrongHelper 0 xs where
    firstWrongHelper n (True:xs) = firstWrongHelper (n + 1) xs
    firstWrongHelper n (False:xs) = n
    firstWrongHelper n _ = -1


main :: IO()
main = do
    print(let x = firstWrong tests in if x == -1 then "TESTS PASSED!" else "FAILED AT TEST: " ++ show x)