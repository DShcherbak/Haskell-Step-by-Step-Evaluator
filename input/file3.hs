combineMap f g arr = map (f.g) arr

combineIncAndDouble arr = combineMap (+1) (*2) arr

guarded n | n > 3     = "Big"
          | n == 2    = "Medium"
          | n /= 1 = "XMedium"
          | otherwise = "Small"