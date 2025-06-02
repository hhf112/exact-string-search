#include <iostream>
#include <string>
#include <thread>
#include <unordered_set>
#include <vector>

#include "BoyreMoore.h"

using namespace std;
using namespace std::chrono;

void BoyreMoore::search(const string &text, const string &pat,
                        std::vector<int> &arr, int l, int r) {
  int s = l, j;
  int m = pat.length();
  int n = text.length();

  vector<int> bpos(m + 1), shift(m + 1);

  for (int i = 0; i < m + 1; i++)
    shift[i] = 0;

  vector<int> badchar(NO_OF_CHARS);
  badCharHeuristic(pat, m, badchar);
  preprocess_strong_suffix(shift, bpos, pat, m);
  preprocess_case2(shift, bpos, pat, m);

  while (s <= r - m) {
    j = m - 1;
    while (j >= 0 && pat[j] == text[s + j])
      j--;

    if (j < 0) {
      arr.push_back(s);
      s += max(shift[0], (s + m < n) ? m - badchar[text[s + m]] : 1);
    } else
      s += max(shift[j + 1], max(1, badchar[text[s + j]]));
  }
}

unordered_set<int> BoyreMoore::parallelSearch(const string &text,
                                              const string &pattern) {
  int n = text.length();
  int m = pattern.length();
  unordered_set<int> retur;
  int chunk = min(PARALLEL * m, (int)std::thread::hardware_concurrency());

  if (n / (PARALLEL * m) == 0) {
    vector<int> sm;
    search(text, pattern, sm, 0, n);
    return retur;
  }

  vector<pair<thread, vector<int>>> v(n / chunk + bool(n % chunk));
  int k = v.size();

  auto strt = high_resolution_clock::now();
  for (int i = 0; i < k; i++) {
    v[i].first = thread([&]() {
      search(text, pattern, v[i].second, i * chunk,
             min((i + 1) * chunk + m, n));

      // search(text, pattern, v[i].second, i * chunk,
      //        min((i + 1) * chunk, n));
    });
  }
  auto en = duration_cast<microseconds>(high_resolution_clock::now() - strt);
  cout << en.count() << '\n';

  std::for_each(v.begin(), v.end(),
                [](pair<thread, vector<int>> &p) { p.first.join(); });

  for (auto &x : v)
    retur.insert(x.second.begin(), x.second.end());

  return retur;
}
