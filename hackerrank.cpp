#include <bits/stdc++.h>
#include <cctype>
#include <ios>
#include <limits>
using namespace std;

using LL = long long int;

#define NO_OF_CHARS 256
void badCharHeuristic(const string &str, int size, int badchar[NO_OF_CHARS]) {
  int i;
  for (i = 0; i < NO_OF_CHARS; i++)
    badchar[i] = -1;

  for (i = 0; i < size; i++)
    badchar[(int)str[i]] = i;
}

vector<int> search(const string &txt, const string &pat, vector<int> &res) {
  int m = pat.size();
  int n = txt.size();

  int badchar[NO_OF_CHARS];
  badCharHeuristic(pat, m, badchar);
  int s = 0;
  while (s <= (n - m)) {
    int j = m - 1;
    while (j >= 0 && pat[j] == txt[s + j])
      j--;
    if (j < 0) {
      res.push_back(s);
      s += (s + m < n) ? m - badchar[(int)txt[s + m]] : 1;
    }

    else
      s += max(1, j - badchar[(int)txt[s + j]]);
  }
  return res;
}

LL eval(const string &bf, int i, int len, int mul) {
  LL res = 0;
  string x = "0", y = "0";
  if (i - 1 >= 0 && bf[i - 1] == '_')
    return 0;
  i += 4;
  while (i < len && isdigit(bf[i])) {
    x.push_back(bf[i]);
    ++i;
  }

  if (i < len && bf[i] == ',') {
    ++i;
    while (i < len && isdigit(bf[i])) {
      y.push_back(bf[i]);
      ++i;
    }

    if (i < len && bf[i] == ')') {
      if (mul)
        res = stoll(x) * stoll(y);
      else
        res = stoll(x) + stoll(y);
    }
  }
  return res;
};

int bns(int l, int r, int val, vector<int> &sr) {
  while (l + 1 < r) {
    int mid = l + (r - l) / 2;

    if (sr[l] < val)
      l = mid;
    else
      r = mid;
  }

  return l;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  LL numlines;
  cin >> numlines;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  string buffer;

  bool add = 1, mul = 1;
  LL res = 0;
  while (numlines--) {
    getline(cin, buffer);
    int s = buffer.length();
    vector<int> dontadd{-1}, dontmul{-1}, domul{-1}, doadd{-1}, mulv, addv;
    search(buffer, "don't_add()", dontadd);
    search(buffer, "don't_mul()", dontmul);
    search(buffer, "do_add()", doadd);
    search(buffer, "do_mul()", domul);
    search(buffer, "mul(", mulv);
    search(buffer, "add(", addv);

    int m = mulv.size(), a = addv.size();
    for (int i = 0; i < m; i++) {
      int d1 = dontmul[bns(0, dontmul.size(), mulv[i], dontmul)];
      int d2 = domul[bns(0, domul.size(), mulv[i], domul)];
      if (d2 == d1 && mul) {
        res += eval(buffer, mulv[i], s, 1);
      } else if (d2 > d1) {
        res += eval(buffer, mulv[i], s, 1);
      }
    }

    for (int i = 0; i < a; i++) {
      int d1 = dontadd[bns(0, dontadd.size(), addv[i], dontadd)];
      int d2 = doadd[bns(0, doadd.size(), addv[i], doadd)];
      if (d2 == d1 && add) {
        res += eval(buffer, addv[i], s, 0);
      } else if (d2 > d1) {
        res += eval(buffer, addv[i], s, 0);
      }
    }

    add = (dontadd[dontadd.size() - 1] > doadd[doadd.size() - 1] ? 0 : 1);

    mul = (dontmul[dontmul.size() - 1] > domul[domul.size() - 1] ? 0 : 1);
  }
  cout << res << '\n';
  return 0;
}
