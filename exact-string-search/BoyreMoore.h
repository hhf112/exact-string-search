#pragma once

#include <fstream>
#include <functional>
#include <string>
#include <thread>
#include <unordered_set>
#include <vector>

class BoyreMoore {
private:
  //  No of characters to be considerd for bad character heuristic.
  const int nchars;

  // size per chunk of filestream in MBs.
  int chunkSize = 50;

  // filepath.
  std::string path;
  std::string buffer;
  std::fstream file;

  // No of shifts for every index preproccessed for Good suffix heuristic
  std::vector<int> shift;
  // border positions preprocessed for Good suffix heuristic
  std::vector<int> bpos;
  std::vector<int> badchar;

  void badCharHeuristic(const std::string &str, int size);
  void preprocess_strong_suffix(const std::string &pat, int m);
  void preprocess_case2(const std::string &pat, int m);

  // > initializes file path.
  // > and sets chunk size to be fetched =  min (passed chunk size, 50 Mb(s)).
  // > opens file.
  void startStream(int chunkSize, const std::string &path);

  //> reads chunks overlapping by patternlength to avoid search misses.
  //> runs action for each chunk.
  void forStream(size_t patternlen,
                 const std::function<void(const std::string &)> &action);

  const std::string &getBuf() { return buffer; }
  std::string getPath() { return path; }

public:
  std::vector<int> find(int chunkSize, const std::string &path,
                        const std::string &pattern);
  std::vector<int> pfind(int chunkSize, const std::string &path,
                         const std::string &pattern);
  std::vector<int> pfind_unique(int chunkSize, const std::string &path,
                                const std::string &pattern);

  BoyreMoore(int nchars) : nchars{nchars} { badchar.resize(nchars, -1); };

  // > Classical Boyre Moore search
  // > shifts the pattern by the maximum of both heuristics.
  void search(const std::string &text, const std::string &pat,
              const std::function<void(int)> &foreach, int l, int r,
              size_t startIndex);

  // > calls search in overlapping partitions of the text.
  // > returns unproccessedstd::vector which can be unordered can have
  // repetitions.
  std::vector<int> parallelSearch(const std::string &text,
                                  const std::string &pattern,
                                  size_t startIndex);
};
