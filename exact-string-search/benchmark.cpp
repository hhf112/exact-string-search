#include <bits/stdc++.h>
#include <chrono>

#include "BoyreMoore.h"
using namespace std;

#define CHUNKSIZE 50 * 1048576
#define BADCHARS 256

using namespace std::chrono;

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "wrong usage\n \t try: srch <filename>\n";
    return 1;
  }

  BoyreMoore bm(BADCHARS);
  std::string pattern(argv[2]);
  std::string filepath(argv[1]);

  std::cout << "chunksize: " << CHUNKSIZE << " byte(s)." << '\n';

  auto strt = high_resolution_clock::now();
  std::vector<int> res1(bm.find(CHUNKSIZE, filepath, pattern));
  auto en = duration_cast<milliseconds>(high_resolution_clock::now() - strt);
  std::cout << "classical search function find: " << en.count() << " ms.\n";
  std::cout << "found: " << res1.size() << '\n';



  strt = high_resolution_clock::now();
  std::vector<int> res2(bm.pfind(CHUNKSIZE, filepath, pattern));
  en = duration_cast<milliseconds>(high_resolution_clock::now() - strt);

  std::cout << "parallel search function pfind: " << en.count() << " ms.\n";
  std::cout << "found: " << res2.size() << '\n';

  strt = high_resolution_clock::now();
  std::vector<int> res3(bm.pfind_unique(CHUNKSIZE, filepath, pattern));
  en = duration_cast<milliseconds>(high_resolution_clock::now() - strt);

  std::cout << "parallel search function pfind_unique: " << en.count()
            << " ms.\n";
  std::cout << "found: " << res3.size() << '\n';
  return 0;
}
