#pragma once

#include <fstream>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_set>
#include <vector>

class BoyreMoore {
private:
  //  No of characters to be considerd for bad character heuristic.
  const size_t nchars;

  // size per chunk of filestream in bytes.
  size_t chunkSize;

  // filepath.
  std::string path;
  std::string buffer;
  std::fstream file;

  // No of shifts for every index preproccessed for Good suffix heuristic
  std::vector<int> shift;
  // border positions preprocessed for Good suffix heuristic
  std::vector<int> bpos;
  std::vector<int> badchar;

  // references for classical Boyre Moore search preprocessing:
  //   https://www.geeksforgeeks.org/boyer-moore-algorithm-good-suffix-heuristic/
  //   https://www.geeksforgeeks.org/boyer-moore-algorithm-for-pattern-searching/
  void badCharHeuristic(const std::string &str, int size);
  void preprocess_strong_suffix(const std::string &pat, int m);
  void preprocess_case2(const std::string &pat, int m);

  // > initializes file path.
  // > and sets chunk size to be fetched =  min (passed chunk size, 50 Mb(s)).
  // > opens file.
  int startStream(size_t chunkSize, const std::string &path);

  //> reads chunks overlapping by patternlength to avoid search misses.
  //> runs action for each chunk.
  void forStream(size_t patternlen,
                 const std::function<void(const std::string &)> &action);

  const std::string &getBuf() { return buffer; }
  std::string getPath() { return path; }

public:
  BoyreMoore(size_t nchars) : nchars{nchars} {
    try {
      badchar.resize(nchars, -1);
    } catch (std::length_error) {
      std::cerr << "BoyreMoore: unable to allcoate badhcars\n";
    };
  };

  // Runs the classical Boyre Moore Search on chunks of the file and returns an
  // ordered and unique value vector of found indexes where the pattern starts.
  std::vector<size_t> find(size_t chunkSize, const std::string &path,
                           const std::string &pattern);

  // Parallelizes classical Boyre Moore on partitions of chunks of the file and
  // returns a possibly unordered vector with duplicate entires.
  std::vector<size_t> pfind(size_t chunkSize, const std::string &path,
                            const std::string &pattern);

  // runs pfind but sorts and deduplicates the result with added overhead.
  std::vector<size_t> pfind_unique(size_t chunkSize, const std::string &path,
                                   const std::string &pattern);

  // > Classical Boyre Moore search
  // > shifts the pattern by the maximum of both heuristics.
  void search(const std::string &text, const std::string &pat,
              const std::function<void(int)> &foreach, int l, int r,
              size_t startIndex);

  // > calls search in overlapping partitions of the text.
  // > returns unproccessedstd::vector which can be unordered can have
  // repetitions.
  std::vector<size_t> parallelSearch(const std::string &text,
                                     const std::string &pattern,
                                     size_t startIndex);
};
