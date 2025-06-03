
# Parallelized Boyre Moore Search
A header only library for parallelized exact string searching using the classical Boyre Moore Algorithm.


# Installation and benchmark build
a simple bash build script (build) is attached to avoid the hassle of object files. 

1. clone the repo and `cd` into it
2. run `sh build`

# Benchmark run 
for the benchmark run: <br>
`./search <filename> <pattern> <chunksize in mbs preferably >= 50>`

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

# Documentation (to be finished soon)
check BoyreMoore.h for concise comments  on every functionality.

### Non parallel find
```
std::vector<int> find(int chunkSize, const std::string &path,
                        const std::string &pattern);
```    
starts a filestream with specified chunk size. 
calls the forStream function 
```
  void forStream(size_t patternlen,
                 const std::function<void(const std::string &)> &action);

```
which implements backwards overlapping by one off of pattern length to avoid misses. passes the classical Boyre Moore Search as a function pointer
```  
void search(const std::string &text, const std::string &pat,
            const std::function<void(int)> &foreach, int l, int r,
            size_t startIndex);
```
for the complete range of the chunk on each chunk. Stores results in a temporary vector.  Concatenates all results in the result vector.

## Parallel find alternatives
```
  std::vector<int> pfind(int chunkSize, const std::string &path,
                         const std::string &pattern);

  //runs pfind but sorts and deduplicates the result with added overhead.
  std::vector<int> pfind_unique(int chunkSize, const std::string &path,
                                const std::string &pattern);

```

## pfind
```
  std::vector<int> pfind(int chunkSize, const std::string &path,
                         const std::string &pattern);
```
starts a file stream using ``


