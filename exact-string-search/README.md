PS. there are some issues where the parallel find function is missing a few results due to search space overlapping, but aside from the few it is fully functional on a large scale.

I am working on fixing it.

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
which implements backwards overlapping by one off of pattern length to avoid misses. calls the classical Boyre Moore Search 
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


