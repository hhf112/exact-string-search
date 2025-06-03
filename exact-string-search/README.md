# Parallelized Boyre Moore Search
A header only library for parallelized exact string searching using the classical Boyre Moore Algorithm.


# Installation and benchmark build
a simple bash build script (build) is attached to avoid the hassle of object files. 

1. clone the repo and `cd` into it
2. run `sh build`

# Benchmark run 
for the benchmark run: <br>
`./search <filename> <pattern>`

#### Sample Output:

```
chunksize: 50 MB(s).
classical search function pfind: 1412 ms.
found: 4032985
parallel search function pfind: 733 ms.
found: 4032983
parallel search function pfind_unique: 866 ms.
found: 4032983
```

# Documentation 
check BoyreMoore.h for concise comments  on every functionality.
