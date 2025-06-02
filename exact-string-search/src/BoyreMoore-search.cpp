#include <functional>
#include <algorithm>
#include <string>
#include <vector>

#include "../BoyreMoore.h"

using namespace std;
using namespace std::chrono;

// BoyreMoore search with maximum skips take between bad character heuristic and
// good suffix heuristic.
void BoyreMoore::search(const string &text, const string &pat,
                        std::function<void(int)> foreach, int l, int r) {
  int patlen = pat.length();
  int n = text.length();
  bpos.resize(patlen + 1), shift.resize(patlen + 1);
  badCharHeuristic(pat, patlen);
  preprocess_strong_suffix(pat, patlen);
  preprocess_case2(pat, patlen);
  int s = l, j;
  while (s <= r - patlen) {
    j = patlen - 1;
    while (j >= 0 && pat[j] == text[s + j])
      j--;
    if (j < 0) {
      foreach (s)
        ;
      s += max(shift[0],
               (s + patlen < n) ? patlen - badchar[text[s + patlen]] : 1);
    } else
      s += max(shift[j + 1], max(1, badchar[text[s + j]]));
  }
}

vector<int> BoyreMoore::parallelSearch(const string &text,
                                       const string &pattern) {

  size_t textlen = text.length();
  size_t patlen = pattern.length();

  // partition allocated to each thread and number of threads.
  size_t part = min(partmul * patlen,
                    (size_t)(textlen / std::thread::hardware_concurrency()));
  int numThreads = (textlen / part + bool(textlen % part));

  std::vector<std::thread> threads(numThreads);
  std::vector<std::vector<int>> results(numThreads);

  //
  for (int i = 0; i < numThreads; i++) {
    threads[i] = thread([&, i]() {
      search(
          // overlapped partition by pattern length.
          text, pattern, [&](int s) { results[i].push_back(s); }, i * part,
          min((i + 1) * part + patlen, textlen));
    });
  }
  std::for_each(threads.begin(), threads.end(),
                [](std::thread &th) { th.join(); });
  //

  // filter result
  std::vector<int> unprocessed;
  size_t possibleTot = 0;
  for (const auto &res : results)
    possibleTot += res.size();

  unprocessed.reserve(possibleTot);

  for (const auto &res : results)
    unprocessed.insert(unprocessed.end(), res.begin(), res.end());

  return unprocessed;
}

vector<int> BoyreMoore::find(int chunkSize, const string &path,
                             const string &pattern) {
  startStream(chunkSize, path);

  vector<int> res;
  forStream([&](const std::string &buf) {
    vector<int> temp(parallelSearch(buffer, pattern));
    res.reserve(temp.size());
    res.insert(res.end(), temp.begin(), temp.end());
  });

  return res;
}

vector<int> BoyreMoore::find_unique(int chunkSize, const string &path,
                                    const string &pattern) {
  startStream(chunkSize, path);

  vector<int> res;
  forStream([&](const std::string &buf) {
    vector<int> temp(parallelSearch(buffer, pattern));
    sort(temp.begin(), temp.end());
    auto en = unique(temp.begin(), temp.end());
    res.reserve(en - temp.begin() );
    res.insert(res.end(), temp.begin(), temp.end());
  });

  return res;
}
