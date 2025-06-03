#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "../BoyreMoore.h"

using namespace std;
using namespace std::chrono;

void BoyreMoore::search(const string &text, const string &pat,
                        const std::function<void(int)> &foreach, int l, int r,
                        size_t startIndex) {
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
      foreach (s + startIndex)
        ;
      s += max(shift[0],
               (s + patlen < n) ? patlen - badchar[text[s + patlen]] : 1);
    } else
      s += max(shift[j + 1], max(1, badchar[text[s + j]]));
  }
}

vector<int> BoyreMoore::parallelSearch(const string &text,
                                       const string &pattern,
                                       size_t startIndex) {

  size_t textlen = text.length();
  size_t patlen = pattern.length();

  if (!std::thread::hardware_concurrency()) {
    std::cerr << "No threads available on system.\n";
    return {};
  }
  size_t part = (textlen / std::thread::hardware_concurrency());
  if (part <= patlen - 1) {
    std::cerr << "input length too small. Consider using classic search.\n";
    return {};
  }
  int numThreads = (textlen / part + bool(textlen % part));

  std::vector<std::thread> threads(numThreads);
  std::vector<std::vector<int>> results(numThreads);

  for (int i = 0; i < numThreads; i++) {
    threads[i] = thread([&, i]() {
      search(
          text, pattern, [&](size_t s) { results[i].push_back(s); }, i * part,
          min((i + 1) * part + patlen - 1, textlen), startIndex);
    });
  }

  size_t tot = 0;
  for (int i = 0; i < numThreads; i++) {
    threads[i].join();
    tot += results[i].size();
  }

  std::vector<int> unprocessed;
  unprocessed.reserve(tot);

  for (const auto &res : results)
    unprocessed.insert(unprocessed.end(), res.begin(), res.end());

  return unprocessed;
}

vector<int> BoyreMoore::find(int chunkSize, const string &path,
                             const string &pattern) {
  if (startStream(chunkSize, path) == 1) return {}; 

  vector<int> res;
  size_t startIndex = 0;
  forStream(pattern.length(), [&](const std::string &buf) {
    vector<int> temp;
    search(
        buf, pattern, [&](int s) { temp.push_back(s); }, 0, buf.length(),
        startIndex);
    res.reserve(res.size() + temp.size());
    res.insert(res.end(), temp.begin(), temp.end());
    startIndex += chunkSize - pattern.length() + 1;
  });

  return res;
}

vector<int> BoyreMoore::pfind(int chunkSize, const string &path,
                              const string &pattern) {
  if (startStream(chunkSize, path) == 1) return {};

  vector<int> res;
  size_t startIndex = 0;
  forStream(pattern.length(), [&](const std::string &buf) {
    vector<int> temp(parallelSearch(buf, pattern, startIndex));

    size_t needed = temp.size() + res.size();
    if (needed > res.capacity()) {
      res.reserve(max(res.capacity() * 2, needed));
    }

    res.insert(res.end(), temp.begin(), temp.end());
    startIndex += chunkSize - pattern.length() + 1;
  });

  return res;
}

vector<int> BoyreMoore::pfind_unique(int chunkSize, const string &path,
                                     const string &pattern) {
  if (startStream(chunkSize, path) == 1) return {}; 

  vector<int> res;
  size_t startIndex = 0;
  forStream(pattern.length(), [&](const std::string &buf) {
    vector<int> temp(parallelSearch(buf, pattern, startIndex));

    res.reserve(res.size() + temp.size());
    res.insert(res.end(), temp.begin(), temp.end());
    startIndex += chunkSize - pattern.length() + 1;
  });

  std::sort(res.begin(), res.end());
  auto en = std::unique(res.begin(), res.end());
  res.erase(en, res.end());

  return res;
}
