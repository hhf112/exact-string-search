#include <bits/stdc++.h>
#include <chrono>
#include <stdexcept>

#include "BoyreMoore.h"
using namespace std;

#define BADCHARS 256

using namespace std::chrono;

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cerr << "wrong usage\n \t try: search <filename> <pattern> <chunksize "
                 "in mbs preferably >= 10>\n";
    return 1;
  }
  int chunksize;
  try {
    chunksize = std::stoll(argv[3]);
  } catch (std::invalid_argument) {
    std::cerr << "invalid chunksize\n";
    exit(1);
  }

  chunksize *= 1024 * 1024;

  BoyreMoore bm(BADCHARS);
  std::string pattern(argv[2]);
  std::string filepath(argv[1]);

  std::cout << "chunksize: " << chunksize << " byte(s)." << '\n';

  auto strt = high_resolution_clock::now();
  std::vector<size_t> res1(bm.find(chunksize, filepath, pattern));
  auto en = duration_cast<milliseconds>(high_resolution_clock::now() - strt);
  std::cout << "classical search function find: " << en.count() << " ms.\n";
  std::cout << "found: " << res1.size() << '\n';

  strt = high_resolution_clock::now();
  std::vector<size_t> res2(bm.pfind(chunksize, filepath, pattern));
  en = duration_cast<milliseconds>(high_resolution_clock::now() - strt);

  std::cout << "parallel search function pfind: " << en.count() << " ms.\n";
  std::cout << "found: " << res2.size() << '\n';

  strt = high_resolution_clock::now();
  std::vector<size_t> res3(bm.pfind_unique(chunksize, filepath, pattern));
  en = duration_cast<milliseconds>(high_resolution_clock::now() - strt);

  std::cout << "parallel search function pfind_unique: " << en.count()
            << " ms.\n";
  std::cout << "found: " << res3.size() << '\n';

  // for (int i = 0; i < min(res1.size(), res3.size()); i++) {
  //   if (res1[i] != res3[i])
  //     std::cout << res1[i] << " != " << res3[i] << '\n';
  // }
  return 0;
}
