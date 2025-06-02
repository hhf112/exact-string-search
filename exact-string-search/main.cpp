#include <bits/stdc++.h>
#include <unordered_set>

#include "BoyreMoore.h"
using namespace std;

#define CHUNKSIZE 10 * 1024
#define BADCHARS 256
#define PARALL 20

using namespace std::chrono;

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "wrong usage\n \t try: srch <filename>\n";
    return 1;
  }

  BoyreMoore bm(BADCHARS, PARALL);
  StreamReader text(CHUNKSIZE, argv[1]);
  std::string pattern(argv[2]);
  std::string buffer;
  buffer.resize(CHUNKSIZE);

  std::cout << "SEARCHSPACE: " << CHUNKSIZE << " byte(s)." << '\n';
  std::vector<int> res;
  auto strt = high_resolution_clock::now();
  text.readChunk(buffer.data());
  bm.search(
      buffer, pattern, [&](int ch) { res.push_back(ch); }, 0, buffer.length());
  auto en = duration_cast<microseconds>(high_resolution_clock::now() - strt);

  std::cout << "regular search function (microseconds): " << en.count() << '\n';
  std::cout << "found: " << res.size() << '\n';

  strt = high_resolution_clock::now();
  std::unordered_set<int> temp(bm.parallelSearch(buffer, pattern));
  en = duration_cast<microseconds>(high_resolution_clock::now() - strt);

  std::cout << "parallel search function (microseconds): " << en.count()
            << '\n';
  std::cout << "found: " << temp.size() << '\n';

  return 0;
}
