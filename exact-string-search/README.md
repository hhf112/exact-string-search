
# Parallelized Boyre Moore Search
A header only library for parallelized exact string searching using the classical Boyre Moore Algorithm.


# Installation and benchmark build
a simple bash build script (build) is attached to avoid the hassle of object files. 

1. clone the repo and `cd` into it
2. run `sh build`

# Benchmark run 
for the benchmark run: <br>
`./search <filename> <pattern> <chunksize in mbs preferably >= 10>`

#### Sample Usage:

```
hrsh $(LAPTOP-HK58DTQE):/mnt/d/dev/poozle/exact-string-search$🌙 ./search 50mb.txt example 20
chunksize: 20971520 byte(s).
classical search function find: 818 ms.
found: 2516583
parallel search function pfind: 409 ms.
found: 2516583
parallel search function pfind_unique: 747 ms.
found: 2516583
```

# Documentation 
check BoyreMoore.h for concise comments  on every functionality.




