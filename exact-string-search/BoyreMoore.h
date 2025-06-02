#pragma once

#include <fstream>
#include <functional>
#include <string>
#include <thread>
#include <unordered_set>
#include <vector>

using namespace std;

class BoyreMoore {
private:
  // CLASSIC BOYRE MOOORE ALGORITHM
  //  No of characters to be considerd for bad character heuristic.
  const int nchars;

  // partmul * patternLength =  partition of search space provided to each
  // thread
  const int partmul;

  // No of shifts for every index preproccessed for Good suffix heuristic
  std::vector<int> shift;

  // border positions preprocessed for Good suffix heuristic
  std::vector<int> bpos;

  // preprocessed by bad character heuristic
  std::vector<int> badchar;

  void badCharHeuristic(const string &str, int size);
  void preprocess_strong_suffix(const string &pat, int m);
  void preprocess_case2(const string &pat, int m);

  // PARALLEL BOYREMOORE
  // size of chunks for reading files.
  int chunkSize;
  // path of the file
  std::string path;
  // chunk storage.
  std::string buffer;
  // file stream
  std::fstream file;

  // opens the file and set the current chunk size to be used by readChunk
  void startStream(int chunkSize, const std::string &path);

  // reads a chunk.
  // either into character array provided or into the buffer.
  bool readChunk(char *put = nullptr);

  // for loop wrapper to run lambda on each chunk
  void forStream(std::function<void(const std::string &)> action);

  const std::string &getBuf() { return buffer; }
  std::string getPath() { return path; }

public:
  BoyreMoore(int nchars, int parallel) : nchars{nchars}, partmul{parallel} {
    badchar.resize(nchars, -1);
  };

  // Non-parallelized search.
  // Boyre Moore which chooses the maximum shift out of both heursitics.
  //
  // foreach lambda is called on every instannce found.
  void search(const string &text, const string &pat,
              std::function<void(int)> foreach, int l, int r);

  // Parallelized Search.
  // To be called on a processed chunk of text to be held in memory.
  // partitions the chunk with overlaps and dispathces threads for each
  // partition.
  //
  // RETURNS: unordered_set of all unique indexes found.
  // patitons are capped by the hardware concurrency of the system.
  vector<int> parallelSearch(const string &text, const string &pattern);

  vector<int> find(int chunkSize, const string &path,
                          const string &pattern);
  vector<int> find_unique(int chunkSize, const string &path,
                          const string &pattern);
};
