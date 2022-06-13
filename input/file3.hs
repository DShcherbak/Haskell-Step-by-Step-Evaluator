parseList [1,2,3,4,5] = [5,4,3,2,1]
parseList xs = [2,3,4]

parseTuple (1,2,3,4,5) = (1,2,3,4,5)
parseTuple xs = (12,23,34,45,56)

combineMap f g arr = map (f.g) arr

combineIncAndDouble arr = combineMap (+1) (*2) arr

guarded n | n > 3     = "Big"
          | n == 2    = "Medium"
          | n /= 1 = "XMedium"
          | otherwise = "Small"


