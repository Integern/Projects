

-------------------------
-------- PART A --------- 
-------------------------

type Var = String

data Term =
    Variable Var
  | Lambda   Var  Term
  | Apply    Term Term
  --deriving Show

instance Show Term where
  show = pretty

example :: Term
example = Lambda "a" (Lambda "x" (Apply (Apply (Lambda "y" (Variable "a")) (Variable "x")) (Variable "b")))

pretty :: Term -> String
pretty = f 0
    where
      f i (Variable x) = x
      f i (Lambda x m) = if i /= 0 then "(" ++ s ++ ")" else s where s = "\\" ++ x ++ ". " ++ f 0 m 
      f i (Apply  n m) = if i == 2 then "(" ++ s ++ ")" else s where s = f 1 n ++ " " ++ f 2 m


------------------------- Assignment 1

numeral_recurse :: Int -> Term
numeral_recurse i
    | i == 0    = Variable "x"
    | i == 1    = Apply (Variable "f") (Variable "x")
    | otherwise = Apply (Variable "f") (numeral_recurse (i-1))
    
    
numeral :: Int -> Term
numeral i = Lambda "f" ( Lambda "x" (numeral_recurse i) )


-------------------------

merge :: Ord a => [a] -> [a] -> [a]
merge xs [] = xs
merge [] ys = ys
merge (x:xs) (y:ys)
    | x == y    = x : merge xs ys
    | x <= y    = x : merge xs (y:ys)
    | otherwise = y : merge (x:xs) ys


------------------------- Assignment 2
alphabet :: [Var]
alphabet = ["a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"]

variables :: [Var]
variables = alphabet ++ [ letter ++ (show i) | i <- [1..], letter <- alphabet]


filterVariables :: [Var] -> [Var] -> [Var] --Returns the first with all shared elements of the second removed.
filterVariables first second = [ x | x <- first, elem x second == False ]


fresh :: [Var] -> Var
fresh inUse = head (filterVariables variables inUse)


used :: Term -> [Var]
used = f
    where
      f (Variable x) = [x]
      f (Lambda x m) = merge [x] (used m)
      f (Apply n m) = merge (used n) (used m)


------------------------- Assignment 3

rename :: Var -> Var -> Term -> Term --Renames x to y in our term
rename x y (Variable z)
    | z == x    = (Variable y)
    | otherwise = (Variable z)
rename x y (Lambda z n)
    | z == x    = (Lambda y (rename x y n))
    | otherwise = (Lambda z (rename x y n))
rename x y (Apply  n m) = (Apply (rename x y n) (rename x y m))


getFreshSubstitute :: Term -> Term -> Var -> Var
getFreshSubstitute n m x = fresh ( merge (merge (used n) (used m)) [x] )


substitute :: Var -> Term -> Term -> Term
substitute x n (Variable y)
    | y == x    = n
    | otherwise = (Variable y)
substitute x n (Lambda y m)
    | y == x    = (Lambda y m)
    | otherwise = (Lambda freshVar (  substitute x n (rename y freshVar m))) 
        where 
            freshVar = getFreshSubstitute n m x
substitute x n (Apply l m) = (Apply (substitute x n l) (substitute x n m))


------------------------- Assignment 4

beta :: Term -> [Term]
beta (Apply (Lambda x n) m) = [substitute x m n] ++
                              [ Apply (beta lambdaTerm !! (z-1)) m | z <- [1..length(beta lambdaTerm)]] ++
                              [ Apply lambdaTerm (beta m !! (z-1)) | z <- [1..length(beta m)]]
    where
        lambdaTerm = Lambda x n
beta (Variable x) = []
beta (Lambda x n) = [ (Lambda x (beta n !! (z-1))) | z <- [1..length(beta n)]]
beta (Apply n m) = [ Apply n (beta m !! (z-1)) | z <- [1..length(beta m)]] ++
                   [ Apply (beta n !! (z-1)) m | z <- [1..length(beta n)]]


normalize :: Term -> IO ()
normalize term = do
    print term
    let newTerms = beta term
    if length(newTerms) > 0
        then normalize (newTerms !! 0)
        else return ()
    


------------------------- 

a_beta :: Term -> [Term]
a_beta (Apply (Lambda x n) m) = [ Apply (a_beta lambdaTerm !! (z-1)) m | z <- [1..length(a_beta lambdaTerm)]] ++
                                [ Apply lambdaTerm (a_beta m !! (z-1)) | z <- [1..length(a_beta m)]] ++ 
                                [substitute x m n]

    where
        lambdaTerm = Lambda x n
a_beta (Variable x) = []
a_beta (Lambda x n) = [ (Lambda x (a_beta n !! (z-1))) | z <- [1..length(a_beta n)]]
a_beta (Apply n m) = [ Apply n (a_beta m !! (z-1)) | z <- [1..length(a_beta m)]] ++
                   [ Apply (a_beta n !! (z-1)) m | z <- [1..length(a_beta n)]]

a_normalize :: Term -> IO ()
a_normalize term = do
    print term
    let newTerms = a_beta term
    if length(newTerms) > 0
        then a_normalize (newTerms !! 0)
        else return ()
        
normalize_check :: Term -> IO()
normalize_check term = do
    print "normalize:"
    normalize term
    print "a_normalize: "
    a_normalize term


-------------------------

example1 :: Term
example1 = Apply (numeral 2) (numeral 2)

example2 :: Term
example2 = Apply (Lambda "a" example) example


-------------------------
-------- PART B --------- 
-------------------------


------------------------- Assignment 5 (PAM)
type PState = (Term, [Term])

state1 = (Lambda "x" (Lambda "y" (Variable "x")) , [Variable "Yes", Variable "No"])

term1 = Apply (Apply (Lambda "x" (Lambda "y" (Variable "x"))) (Variable "Yes")) (Variable "No")

term2 = Apply (Apply (Lambda "b" (Apply example (Variable "Yes"))) (Lambda "z" (Variable "z"))) (Variable "No")


p_start :: Term -> PState
p_start term = (term, [])

p_step :: PState -> PState
p_step ( (Lambda x n) , (m:ms)) = ( (substitute x m n), ms)
p_step ( (Apply n m), stack) = (n, [m] ++ stack)
p_step (term, stack) = (term, stack)

p_final :: PState -> Bool
p_final ( (Lambda x n), [] ) = True
p_final ( (Variable x), stack)  = True
p_final state = False

p_run :: Term -> IO ()
p_run term = do
    let state = p_start term
    doRun state
        where
            doRun :: PState -> IO ()
            doRun state = do
                print state
                if p_final state
                    then print (p_readback state)
                    else doRun (p_step state)
    
p_readback :: PState -> Term
p_readback (term, stack) = makeTerm ([term] ++ stack)
    where
        makeTerm :: [Term] -> Term
        makeTerm (t:[]) = t
        makeTerm (t:ts) = Apply t (makeTerm ts)



------------------------- Assignment 6 (KAM)
data Env =
    Cons (Var, Term, Env) Env
  | Nil
  
instance Show Env where
    show (Cons (x, n, a) b) = "[(" ++ (show x) ++ "," ++ (show n) ++ "," ++ (show a) ++ ")]"
    show Nil = "[]"
  
type State = (Term, Env, [Closure])
type Closure = (Term, Env)

state2 = ( Apply (Lambda "x" (Variable "x")) (Variable "y"), (Cons ( ("y"), (Lambda "z" (Variable "z")), Nil ) Nil), Nil )

state3 = (( Apply (Variable "x") (Variable "x")), (Cons ("x", (Apply (Lambda "x" (Variable "x")) (Variable "x")), Nil) Nil), Nil)

--state4 = ( (Lambda "y" (Variable "x")), (Nil), (Cons ( (Variable "z"), (Cons ( "z", (Lambda "a" (Variable "b")), (Cons ("b", (Variable "c"), Nil) Nil) ) Nil) ) Nil) )

{-
start :: Term -> State
start = undefined

step :: State -> State
step = undefined

final :: State -> Bool
final = undefined

run :: Term -> IO ()
run = undefined

readback :: State -> Term
readback = undefined
-}

