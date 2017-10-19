#include <bits/stdc++.h>
using namespace std;

const int maxn = 2010;

long long arr[maxn];
long long asum[maxn];
long long dp[maxn][maxn];
int s[maxn][maxn];
char sv[maxn][maxn];
long long svl[maxn];

void myfill(long long st, long long ed, char c) {
  for(long long i = st; i < ed; i++) {
    sv[i][svl[i]++] = c;
  }
  if(st + 1 >= ed) return;
  myfill(st, s[st][ed], '0');
  myfill(s[st][ed], ed, '1');
}

int main() {
  long long n;
  scanf("%lld", &n);
  for(long long i = 0; i < n; i++) {
    scanf("%lld", &arr[i]);
  }
  partial_sum(arr, arr + n, asum + 1);
  memset(dp, 0x3f, sizeof dp);
  for(long long l = 0; l <= n; l++) {
    for(long long i = 0; i + l <= n; i++) {
      // we are going to solve dp[i][i + l]
      if(l < 2) dp[i][i + l] = 0;
      else if(l == 2) {
        dp[i][i + l] = arr[i] + arr[i + 1];
        s[i][i + l] = i + 1;
      } else {
        for(long long k = s[i][i + l - 1]; k <= s[i + 1][i + l]; k++) {
          long long x = asum[i + l] - asum[i] + dp[i][k] + dp[k][i + l];
          if(x < dp[i][i + l]) {
            dp[i][i + l] = x;
            s[i][i + l] = k;
          }
        }
      }
    }
  }
  myfill(0, s[0][n], '0');
  myfill(s[0][n], n, '1');
  for(long long i = 0; i < n; i++) {
    printf("%s\n", sv[i]);
  }
  return 0;
}
