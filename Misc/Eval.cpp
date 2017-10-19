#include <bits/stdc++.h>

using namespace std;

// Q ::= E=E
// E ::= T | E+T | E-T
// T ::= F | T*F
// F ::= N | -F | (E)
// N ::= 0 | 1B
// B ::= ε | 0B | 1B
// Left associative

struct Node {
  int v;
  Node *l, *r;
  Node(int v = 0, Node* l = nullptr, Node* r = nullptr):
  v(v), l(l), r(r){}

  int solve() {
    int lv, rv;
    if(l) lv = l->solve();
    if(r) rv = r->solve();
    if(!l && !r) {
      return v;
    } else if(v == '=') {
      return lv == rv;
    } else if(v == '+') {
      return lv + rv;
    } else if(v == '*') {
      return lv * rv;
    } else if(v == '-') {
      if(r) return lv - rv;
      else return -lv;
    }
    throw invalid_argument("Bad tree");
  }

  void print(int depth) {
    if(l) l->print(depth + 1);
    for(int i = 0; i < depth; i++) {
      putchar(' ');
    }
    if(!l && !r) printf("%d\n", v);
    else printf("%c\n", v);
    if(r) r->print(depth + 1);
  }
};

Node* Q();
Node* E();
Node* T();
Node* F();
Node* N();
char B();
char consume(string s);
int cur = 0;
string expr;

char consume(string s) {
  for(auto c: s) {
    if(expr[cur] == c) {
      cur++;
      return c;
    }
  }
  throw invalid_argument(string("Expect ") + s + string(", but met ") + expr[cur]);
}

char consumeif(string s) {
  for(auto c: s) {
    if(expr[cur] == c) {
      cur++;
      return c;
    }
  }
  return 0;
}

Node* Q() {
  auto n1 = E();
  consume("=");
  auto n2 = E();
  consume("#");
  return new Node('=', n1, n2);
}

Node* E() {
  Node* ret = nullptr;
  while(1) {
    if(!ret) ret = T();
    else ret->r = T();
    char n;
    if(!(n = consumeif("+-"))) return ret;
    else ret = new Node(n, ret);
  }
}

Node* T() {
  Node* ret = nullptr;
  while(1) {
    if(!ret) ret = F();
    else ret->r = F();
    char n;
    if(!(n = consumeif("*"))) return ret;
    else ret = new Node(n, ret);
  }
}

Node* F() {
  if(consumeif("-")) {
    return new Node('-', F());
  } else if(consumeif("(")) {
    Node* ret = E();
    consume(")");
    return ret;
  } else {
    return N();
  }
}

Node* N() {
  int v = 0;
  if(consumeif("0")) {
    return new Node(0);
  } else {
    v = consume("01") - '0';
    int x;
    while((x = B())) {
      v *= 2;
      v += x - '0';
    }
    return new Node(v);
  }
}

char B() {
  return consumeif("01");
}

bool solve(string e) {
  try {
    expr = e + '#';
    cur = 0;
    return Q() -> solve();
  } catch(...) {
    return 0;
  }
}

char keyWords[] = {'+', '-', '*', '(', ')', '1', '0', '='};
bool keyVis[10];
char words[10];
vector<string> mp;

void dfs(int deep, int len) {
  if(deep >= len) {
    string res = "";
    for(int i = 0; i < 8; i++) {
      res += words[i];
    }
    mp.push_back(res);
  } else {
    for(int i = 0; i < 8; i++) {
      if(!keyVis[i]) {
        keyVis[i] = 1;
        words[deep] = keyWords[i];
        dfs(deep + 1, len);
        keyVis[i] = 0;
      }
    }
  }
}

int check(string s) {
  string s2 = s;
  sort(s2.begin(), s2.end());
  s2.erase(unique(s2.begin(), s2.end()), s2.end());
  vector<char> chs;
  for(auto ch: s2) {
    if(isalpha(ch)) {
      chs.push_back(ch);
    }
  }
  if(chs.size() > 8) return 0;
  dfs(0, chs.size());
  int res = 0;
  for(auto str: mp) {
    map<char, char> trans;
    for(int i = 0; i < (int)chs.size(); i++) {
      trans[chs[i]] = str[i];
    }
    string afterChange = s;
    for(auto&& ch: afterChange) {
      if(isalpha(ch)) ch = trans[ch];
    }
    if(solve(afterChange)) res++;
  }
  return res;
}

int32_t main() {
  string s;
  cin >> s;
  printf("%d\n", check(s));
  return 0;
}