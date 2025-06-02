#include <bits/stdc++.h>
#include <cctype>
#include <ios>
using namespace std;

using LL = long long int;
// Boyre Moore exact string search.
#define NO_OF_CHARS 256
void badCharHeuristic(const string &str, int size, int badchar[NO_OF_CHARS]) {
  int i;
  for (i = 0; i < NO_OF_CHARS; i++)
    badchar[i] = -1;
  for (i = 0; i < size; i++)
    badchar[(int)str[i]] = i;
}

void search(const string &txt, const string &pat, auto dothen) {
  int m = pat.size();
  size_t n = txt.size();
  int badchar[NO_OF_CHARS];
  badCharHeuristic(pat, m, badchar);
  size_t s = 0;
  while (s <= (n - m)) {
    int j = m - 1;
    while (j >= 0 && pat[j] == txt[s + j])
      j--;
    if (j < 0) {
      dothen(s);
      s += (s + m < n) ? m - badchar[(int)txt[s + m]] : 1;
    } else
      s += max(1, j - badchar[(int)txt[s + j]]);
  }
}

int main() {
  bool canAdd = 1, canMul = 1;
  LL n;
  cin >> n;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  string buf;

  LL res = 0;
  auto eval = [&](const string &bf, size_t s, bool flag) {
    if (flag == 0 && canAdd == 0)
      return;
    else if (flag == 1 && canMul == 0)
      return;

    string x = "0", y = "0";
    size_t len = bf.length();
    while (s < len && isdigit(bf[s]))
      x += bf[s++];

    if (s < len && bf[s++] == ',') {

      while (s < len && isdigit(bf[s]))
        y += bf[s++];

      if (s < len && bf[s] == ')') {
        res += (flag ? stoll(x) * stoll(y) : stoll(x) + stoll(y));
      }
    }
  };

  while (n--) {
    getline(cin, buf);
    size_t len = buf.length();

    search(buf, "add(", [&](size_t s) {
      if (s + 4 < len && buf[s + 4] == ')') {
        if (s >= 3 && buf.substr(s - 3, 3) == "do_")
          canAdd = 1;
        else if (s >= 6 && buf.substr(s - 6, 6) == "don't_")
          canAdd = 0;
      } else
        eval(buf, s + 4, 0);
    });

    search(buf, "mul(", [&](size_t s) {
      if (s + 4 < len && buf[s + 4] == ')') {
        if (s >= 3 && buf.substr(s - 3, 3) == "do_")
          canMul = 1;
        else if (s >= 6 && buf.substr(s - 6, 6) == "don't_")
          canMul = 0;
      } else
        eval(buf, s + 4, 1);
    });
  }

  cout << res << '\n';
  return 0;
}
