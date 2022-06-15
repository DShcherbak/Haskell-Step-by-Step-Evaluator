map :: (a -> b) -> [a] -> [b]
map f [] = []
map f (x:xs) = f x : map f xs

parseList :: [Int] -> [Int]
parseList [1,2,3,4,5] = [5,4,3,2,1]
parseList xs = [2,3,4]

parseTuple :: (Int, Int, Int, Int, Int) -> (Int, Int, Int, Int, Int)
parseTuple (1,2,3,4,5) = (1,2,3,4,5)
parseTuple xs = (12,23,34,45,56)

combineMap :: (a -> b) -> (b -> c) -> [a] -> [c]
combineMap f g arr = map (f.g) arr

combineMap :: [Int] -> [Int]
combineIncAndDouble arr = combineMap (+1) (*2) arr

guarded :: Int -> String
guarded n | n > 3     = "Big"
          | n == 2    = "Medium"
          | n /= 1 = "XMedium"
          | otherwise = "Small"


