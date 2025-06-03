//PS: I was trying to do this using Boyre moore search which turned out to be not 
//feasible for adjusting variable skips. 
//
#include <bits/stdc++.h>
#include <cctype>
#include <ios>
using namespace std;

using LL = long long int;
int main() {
  bool canAdd = 1, canMul = 1;
  LL n;
  cin >> n;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  string buf;

  LL res = 0;
auto parseAndEval = [&](const string &bf, size_t &s, bool flag) {
    s += 4;   
    string x = "0", y = "0";
    size_t len = bf.length();
    while (s < len && isdigit(bf[s]))
      x += bf[s++];

    if (s < len && bf[s++] == ',') {

      while (s < len && isdigit(bf[s]))
        y += bf[s++];

      if (s < len && bf[s] == ')') {
        ++s;
        if (flag == 0 && canAdd == 0)
          return;
        else if (flag == 1 && canMul == 0)
          return;
        res += (flag ? stoll(x) * stoll(y) : stoll(x) + stoll(y));
      }
    }
  };

  while (n--) {
    getline(cin, buf);

    size_t i = 0;
    while (i < buf.size()) {
      if (buf.compare(i, 8, "do_add()") == 0) {
        canAdd = true;
        i += 8;
      } else if (buf.compare(i, 11, "don't_add()") == 0) {
        canAdd = false;
        i += 11;
      } else if (buf.compare(i, 4, "add(") == 0) {
        parseAndEval(buf, i, 0);
      } else if (buf.compare(i, 8, "do_mul()") == 0) {
        canMul = true;
        i += 8;
      } else if (buf.compare(i, 11, "don't_mul()") == 0) {
        canMul = false;
        i += 11;
      } else if (buf.compare(i, 4, "mul(") == 0) {
        parseAndEval(buf, i, 1);
      } else {
        i++;
      }
    }
  }

  cout << res << '\n';
  return 0;
}
