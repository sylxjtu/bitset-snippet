#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>

typedef unsigned long long ull;

const int ratio = 7;
const int MAXN = 3000005;
const unsigned int base = 2333;

using namespace std;

struct Node {
  int size, val;
  Node *ls, *rs;
  Node() {}
  Node(const int &s, const int &v, Node *a, Node *b): size(s), val(v), ls(a), rs(b) {}
}*root, *father, *null, *rt, *st[2000000], t[2000000];
  
char s[MAXN];
ull rk[MAXN];
int len, c[MAXN], strlenth, segNode;
unsigned int power[MAXN], hash1[MAXN], hash2[MAXN];

int mask;

ull getHash(unsigned int hash[], int l, int r) {
  return hash[r] - hash[l - 1] * power[r - l + 1];
}

Node *newNode(int s, int v, Node *a, Node *b) {
  return &(*st[segNode++] = Node(s, v, a, b));
}

Node *merge(Node *a, Node *b) {
  return newNode(a->size + b->size, b->val, a, b);
}

void update(Node *cur) {
  if(cur->ls->size) {
    cur->size = cur->ls->size + cur->rs->size;
    cur->val = cur->rs->val;
  }
}

bool cmp(const int & x, const int & y) {
  return c[x] > c[y] || (c[x] == c[y] && rk[x - 1] > rk[y - 1]);
}
  
bool cmp(const int & x) {
  int l = 1, r = min(strlenth, x), mid, ans = 0;
  while(l <= r) {
    mid = l + r >> 1;
    if(getHash(hash1, x - mid + 1, x) == getHash(hash2, strlenth - mid + 1, strlenth)) {
      l = mid + 1, ans = mid;
    } else {
      r = mid - 1;
    }
  }
  if(ans == x) return 1;
  if(ans == strlenth) return 0;
  return c[x - ans] < s[strlenth - ans];
}
  
void rebuild(Node *cur, const ull l, const ull r) {
  if(cur->size == 1) rk[cur->val] = l + r;
  else rebuild(cur->ls, l, l + r >> 1), rebuild(cur->rs, l + r >> 1, r);
}
  
void maintain(Node *cur, const ull l, const ull r) {
  if(cur->ls->size > cur->rs->size * ratio) {
    cur->rs = merge(cur->ls->rs, cur->rs);
    st[--segNode] = cur->ls;
    cur->ls = cur->ls->ls;
    rebuild(cur, l, r);
  } 
  if(cur->rs->size > cur->ls->size * ratio) {
    cur->ls = merge(cur->ls, cur->rs->ls);
    st[--segNode] = cur->rs;
    cur->rs = cur->rs->rs;
    rebuild(cur, l, r);
  }
}
  
void insert(int x, Node *cur, const ull l, const ull r) {
  if(cur->size == 1) {
    if(!cmp(x, cur->val)) {
      cur->ls = newNode(1, x, null, null);
      rk[x] = l + (l + r >> 1);
      cur->rs = newNode(1, cur->val, null, null);
      rk[cur->val] = (l + r >> 1) + r;
    } else {
      cur->ls = newNode(1, cur->val, null, null);
      rk[cur->val] = l + (l + r >> 1);
      cur->rs = newNode(1, x, null, null);
      rk[x] = (l + r >> 1) + r;    
    }
  } else if(cmp(x, cur->ls->val)) {
    insert(x, cur->rs, l + r >> 1, r);
  } else {
    insert(x, cur->ls, l, l + r >> 1);
  }
  update(cur), maintain(cur, l, r);
}
  
void erase(int x, Node *cur, const ull l, const ull r) {
  if(cur->size == 1) {
    *father = cur == father->ls ? *father->rs : *father->ls;
  } else {
    father = cur;
    if(cmp(x, cur->ls->val)) {
      erase(x, cur->rs, l + r >> 1, r);
    } else {
      erase(x, cur->ls, l, l + r >> 1);
    }
  }
  update(cur), maintain(cur, l, r);
}
  
int find(Node *cur) {
  if(cur->size == 1) return cmp(cur->val);
  if(cmp(cur->ls->val)) return find(cur->rs) + cur->ls->size;
  return find(cur->ls);
}
  
void add(const char &x) {
  c[++len] = x;
  hash1[len] = hash1[len - 1] * base + x;
  insert(len, root, 0, 1ll << 62);
}
  
void erase(int m) {
  while(m--) erase(len, root, 0, 1ll << 62), c[len--] = 0;
}
  
void decode(char s[], int mask) {
  strlenth = strlen(s);
  for(int i = 0 ; s[i] ; i++) {
    mask = (mask * 131 + i) % strlenth;
    swap(s[i], s[mask]);
  }
}

void segInit() {
  segNode = 0;
  for(int i = 0; i < 2000000; i++) st[i] = &t[i];
  null = newNode(0, 0, 0, 0);
  root = newNode(1, 1, null, null);
}

int main() {
  segInit();
  int q; scanf("%d", &q);
  scanf("%s", s); 
  power[0] = 1;
  for(int i = 1; i <= 3000000; i++) {
    power[i] = power[i - 1] * base;
  }
  c[len = 1] = 0;
  for(int i = 0; s[i]; i++) add(s[i]);
  while(q--) {
  char opt[10]; scanf("%s", opt);
    if(opt[0] == 'A') {
      scanf("%s", s); decode(s, mask);
      for(int i = 0; s[i]; i++) add(s[i]);
    } else if(opt[0] == 'D') {
      int id; scanf("%d", &id);
      erase(id);
    } else {
      scanf("%s", s + 2); decode(s + 2, mask);
      strlenth++;
      hash2[1] = s[1] = 127;
      for(int i = 2; s[i]; i++) {
        hash2[i] = hash2[i - 1] * base + s[i];
      }
      int ans = find(root);
      for(int i = 1; i <= strlenth; i++) s[i] = s[i + 1];
      s[strlenth--] = 0;
      for(int i = 1; s[i]; i++) {
        hash2[i] = hash2[i - 1] * base + s[i];
      }
      ans -= find(root);
      printf("%d\n", ans);
      mask ^= ans;
    }
  }
  return 0;
}