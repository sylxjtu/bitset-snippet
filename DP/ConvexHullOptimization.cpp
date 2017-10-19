#include <bits/stdc++.h>
using namespace std;

// dp[i] = min(dp[j] + (S[i] - S[j])^2) + M
// dp[0] = 0;
typedef long long ll;
ll dp[500010];
ll S[500010];

inline ll gety(int x) {return dp[x] + S[x] * S[x];}
inline ll getx(int x) {return 2 * S[x];}

int main() {
  int n; ll m;
  while (scanf("%d%lld", &n, &m) == 2) {
    deque<int> mq;
    for(int i = 1; i <= n; i++) {
      ll x;
      scanf("%lld", &x);
      S[i] = S[i - 1] + x;
    }
    mq.push_back(0);
    for(int i = 1; i <= n; i++) {
      while(1) {
        int a = mq.front();
        if(mq.size() == 1) {
          dp[i] = dp[a] + (S[i] - S[a]) * (S[i] - S[a]) + m;
          break;
        }
        mq.pop_front(); int b = mq.front();
        if(gety(b) - gety(a) <= S[i] * (getx(b) - getx(a))) continue;
        else {
          mq.push_front(a);
          dp[i] = dp[a] + (S[i] - S[a]) * (S[i] - S[a]) + m;
          break;
        }
      }
      while(1) {
        int a = mq.back();
        if(mq.size() == 1) {
          mq.push_back(i);
          break;
        }
        mq.pop_back(); int b = mq.back();
        if((gety(i) - gety(a)) * (getx(a) - getx(b)) > (gety(a) - gety(b)) * (getx(i) - getx(a))) {
          mq.push_back(a);
          mq.push_back(i);
          break;
        }
      }
    }
    printf("%lld\n", dp[n]);
  }
  return 0;
}
