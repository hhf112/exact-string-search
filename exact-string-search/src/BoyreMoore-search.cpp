#include <algorithm>
#include <functional>
#include <iostream>
#include <limits.h>
#include <string>
#include <vector>

#include "../BoyreMoore.h"

using namespace std::chrono;

void BoyreMoore::search(const std::string &text, const std::string &pat,
                        const std::function<void(int)> &foreach, int l, int r,
                        size_t startIndex) {

  if (pat.length() > INT_MAX) {
    std::cerr << "search: pattern length too long\n";
    return;
  }
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
      s += std::max(shift[0],
                    (s + patlen < n) ? patlen - badchar[text[s + patlen]] : 1);
    } else
      s += std::max(shift[j + 1], std::max(1, badchar[text[s + j]]));
  }
}

std::vector<size_t> BoyreMoore::parallelSearch(const std::string &text,
                                               const std::string &pattern,
                                               size_t startIndex) {

  size_t textlen = text.length();
  if (pattern.length() > INT_MAX) {
    std::cerr << "parallelSearch: pattern length too long\n";
    return {};
  }
  size_t patlen = pattern.length();

  if (!std::thread::hardware_concurrency()) {
    std::cerr << "parallelSearch: No threads available on system.\n";
    return {};
  }

  size_t part = (textlen / std::thread::hardware_concurrency());
  if (part <= patlen - 1) {
    std::cerr << "parallelSearch: input length too small. Consider using "
                 "classic search.\n";
    return {};
  }
  int numThreads = (textlen / part + bool(textlen % part));

  std::vector<std::thread> threads(numThreads);
  std::vector<std::vector<size_t>> results(numThreads);

  for (int i = 0; i < numThreads; i++) {
    threads[i] = std::thread([&, i]() {
      search(
          text, pattern, [&](size_t s) { results[i].push_back(s); }, i * part,
          std::min((i + 1) * part + patlen - 1, textlen), startIndex);
    });

  }

  size_t tot = 0;
  for (int i = 0; i < numThreads; i++) {
    threads[i].join();
    tot += results[i].size();
  }

  std::vector<size_t> unprocessed;
  unprocessed.reserve(tot);

  for (const auto &res : results)
    unprocessed.insert(unprocessed.end(), res.begin(), res.end());

  return unprocessed;
}

std::vector<size_t> BoyreMoore::find(size_t chunkSize, const std::string &path,
                                     const std::string &pattern) {
  if (startStream(chunkSize, path) == 1)
    return {};

  if (!badchar.size()) {
    std::cerr << "find: badchars not initialized\n";
    return {};
  }

  if (chunkSize <= pattern.length()) {
    std::cerr << "find: chunk size too small.\n";
    return {};
  }
  if (pattern.length() > INT_MAX) {
    std::cerr << "find: pattern length too long\n";
    return {};
  }

  std::vector<size_t> res;
  size_t startIndex = 0;
  forStream(pattern.length(), [&](const std::string &buf) {
    std::vector<size_t> temp;
    search(
        buf, pattern, [&](int s) { temp.push_back(s); }, 0, buf.length(),
        startIndex);
    res.reserve(res.size() + temp.size());
    res.insert(res.end(), temp.begin(), temp.end());

    startIndex += chunkSize - pattern.length() + 1;
  });

  return res;
}

std::vector<size_t> BoyreMoore::pfind(size_t chunkSize, const std::string &path,
                                      const std::string &pattern) {

  if (!badchar.size()) {
    std::cerr << "pfind: badchars not initialized\n";
    return {};
  }
  if (startStream(chunkSize, path) == 1)
    return {};
  if (chunkSize <= pattern.length()) {
    std::cerr << "pfind: chunk size too small.\n";
    return {};
  }

  if (pattern.length() > INT_MAX) {
    std::cerr << "pfind: pattern length too long\n";
    return {};
  }

  std::vector<size_t> res;
  size_t startIndex = 0;
  forStream(pattern.length(), [&](const std::string &buf) {
    std::vector<size_t> temp(parallelSearch(buf, pattern, startIndex));

    size_t needed = temp.size() + res.size();
    if (needed > res.capacity()) {
      res.reserve(std::max(res.capacity() * 2, needed));
    }

    res.insert(res.end(), temp.begin(), temp.end());
    startIndex += chunkSize - pattern.length() + 1;
  });

  return res;
}

std::vector<size_t> BoyreMoore::pfind_unique(size_t chunkSize,
                                             const std::string &path,
                                             const std::string &pattern) {

  if (!badchar.size()) {
    std::cerr << "pfind_unique: badchars not initialized\n";
    return {};
  }
  if (startStream(chunkSize, path) == 1)
    return {};

  if (chunkSize <= pattern.length()) {
    std::cerr << "pfind_unique: chunk size too small.\n";
    return {};
  }
  if (pattern.length() > INT_MAX) {
    std::cerr << "pfind_unique: pattern length too long.\n";
    return {};
  }

  std::vector<size_t> res;
  size_t startIndex = 0;
  forStream(pattern.length(), [&](const std::string &buf) {
    std::vector<size_t> temp(parallelSearch(buf, pattern, startIndex));

    res.reserve(res.size() + temp.size());
    res.insert(res.end(), temp.begin(), temp.end());
    startIndex += chunkSize - pattern.length() + 1;
  });

  std::sort(res.begin(), res.end());
  auto en = std::unique(res.begin(), res.end());
  res.erase(en, res.end());

  return res;
}
