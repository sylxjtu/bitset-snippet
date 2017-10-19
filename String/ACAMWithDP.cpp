#include <bits/stdc++.h>
using namespace std;

const int maxl = 1000010;
const int maxc = 26;
int cnt = 0;
char buf[maxl];

struct TrieNode{
  int epn, fail, mfail;
  int children[maxc];
  TrieNode(): epn(0), fail(0), mfail(0) {
    memset(children, 0, sizeof children);
  }
};

vector<TrieNode> trie;

void insert(char* s) {
  int cur = 1;
  for(int i = 0; s[i]; i++) {
    if(trie[cur].children[s[i] - 'a']) cur = trie[cur].children[s[i] - 'a'];
    else {
      trie.emplace_back();
      cur = trie[cur].children[s[i] - 'a'] = trie.size() - 1;
    }
  }
  trie[cur].epn++;
}

void build() {
  queue<int> bfsq;
  bfsq.push(1);
  while (!bfsq.empty()) {
    int node = bfsq.front(); bfsq.pop();
    for(int i = 0; i < maxc; i++) {
      int& nextNode = trie[node].children[i];
      if(nextNode) {
        bfsq.push(nextNode);
        trie[nextNode].fail = trie[trie[node].fail].children[i];
        trie[nextNode].mfail = trie[nextNode].epn ? nextNode : trie[trie[nextNode].fail].mfail;
      } else {
        nextNode = trie[trie[node].fail].children[i];
      }
    }
  }
}

int query(char* s) {
  int cur = 1;
  int ret = 0;
  for(int i = 0; s[i]; i++) {
    cur = trie[cur].children[s[i] - 'a'];
    int j = trie[cur].mfail;
    while (j) {
      ret += trie[j].epn;
      trie[j].epn = 0;
      j = trie[j].mfail = trie[trie[j].fail].mfail;
    }
  }
  return ret;
}

bool trans(int& cur, char c) {
  int ret = 0;
  cur = trie[cur].children[c - 'a'];
  int j = trie[cur].mfail;
  while (j) {
    ret += trie[j].epn;
    // trie[j].epn = 0;
    j = trie[trie[j].fail].mfail;
    // j = trie[j].mfail = trie[trie[j].fail].mfail;
  }
  return ret == 0;
}

// haveint havelow haveup len acstatus
long long dp[2][2][2][30][1010];
vector<pair<char, char>> cs;
const long long md = 1e6+3;

void init() {
  cnt = 0;
  trie.clear();
  trie.resize(2);
  for(int i = 0; i < maxc; i++) {
    trie[0].children[i] = 1;
  }
  for(int i = 'A'; i <= 'Z'; i++) {
    cs.emplace_back(i, i - 'A' + 'a');
  }
  for(int i = 'a'; i <= 'z'; i++) {
    cs.emplace_back(i, i);
  }
  for(int i = '0'; i <= '9'; i++) {
    char x = i;
    char y = '?';
    if(x == '0') y = 'o';
    if(x == '1') y = 'i';
    if(x == '3') y = 'e';
    if(x == '5') y = 's';
    if(x == '7') y = 't';
    cs.emplace_back(x, y);
  }
}

int main() {
  int a, b;
  int n;
  init();
  scanf("%d%d%d", &a, &b, &n);
  for(int i = 0; i < n; i++) {
    scanf("%s", buf);
    insert(buf);
  }
  build();
  long long ans = 0;
  dp[0][0][0][0][1] = 1;
  for(int haveint = 0; haveint <= 1; haveint++) {
    for(int havelow = 0; havelow <= 1; havelow++) {
      for(int haveup = 0; haveup <= 1; haveup++) {
        for(int len = 0; len <= b; len++) {
          for(int status = 1; status < (int)trie.size(); status++) {
            // printf("dp[%d][%d][%d][%d][%d] = %lld\n", haveint, havelow, haveup, len, status, dp[haveint][havelow][haveup][len][status]);
            if(haveint && havelow && haveup && a <= len && len <= b)
              ans = (ans + dp[haveint][havelow][haveup][len][status]) % md;
              // ans = (ans + dp[haveint][havelow][haveup][len][status]);
            for(auto&& p: cs) {
              int nstatus = status;
              if(p.second != '?') {
                if(!trans(nstatus, p.second)) {
                  continue;
                }
              } else {
                nstatus = 1;
              }
              dp
                [haveint || ('0' <= p.first && p.first <= '9')]
                [havelow || ('a' <= p.first && p.first <= 'z')]
                [haveup || ('A' <= p.first && p.first <= 'Z')]
                [len + 1]
                [nstatus]
              += dp[haveint][havelow][haveup][len][status];
              dp
                [haveint || ('0' <= p.first && p.first <= '9')]
                [havelow || ('a' <= p.first && p.first <= 'z')]
                [haveup || ('A' <= p.first && p.first <= 'Z')]
                [len + 1]
                [nstatus]
              %= md;
            }
          }
        }
      }
    }
  }
  printf("%lld\n", ans);
  return 0;
}