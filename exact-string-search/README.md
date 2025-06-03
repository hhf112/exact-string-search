# Parallelized Boyre Moore Search
A header only library for parallelized exact string searching using the classical Boyre Moore Algorithm.


# Installation and benchmark build
a simple bash build script (build) is attached to avoid the hassle of object files. 

1. clone the repo and `cd` into it
2. run `sh build`

# Benchmark run 
for the benchmark run: <br>
`./search <filename> <pattern> <chunksize in mbs >= 50>`

#### Sample Output:

```
chunksize: 52428800 byte(s).
classical search function find: 5036 ms.
found: 4032985
parallel search function pfind: 1111 ms.
found: 4032979
parallel search function pfind_unique: 1245 ms.
found: 4032979
```

# Documentation 
check BoyreMoore.h for concise comments  on every functionality.

`find()`

- [ ] Fix 
