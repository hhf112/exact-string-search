#pragma once

#include <string>
#include <thread>
#include <unordered_set>
#include <vector>

using namespace std;

class BoyreMoore {
private:
  const int NO_OF_CHARS;
  const int PARALLEL;

  void badCharHeuristic(const string &str, int size, vector<int> &badchar);

  void preprocess_strong_suffix(vector<int> &shift, vector<int> &bpos,
                                const string &pat, int m);

  void preprocess_case2(vector<int> &shift, vector<int> &bpos,
                        const string &pat, int m);

public:
  BoyreMoore(int nchars, int parallel)
      : NO_OF_CHARS{nchars}, PARALLEL{parallel} {};

  void search(const string &text, const string &pat, vector<int> &arr, int l,
              int r);

  unordered_set<int> parallelSearch(const string &text, const string &pattern);
};
