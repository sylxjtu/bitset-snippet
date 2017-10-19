#include <bits/stdc++.h>
#define FOR(i, n) for (int (i) = 0; (i) < (n); (i)++)
#define FOR1(i, n) for (int (i) = 1; (i) <= (n); (i)++)

using namespace std;

typedef long long LL;

const int MAXN = 1e6;

LL a[MAXN+10], mx[MAXN+10], pre[MAXN+10];

int main() {
  a[0] = 0, a[1] = 1;
  mx[0] = 0, mx[1] = 1;
  int t, n, k;
  scanf("%d", &t);
  FOR1(ii, t) {
    scanf("%d%d", &n, &k);
    int p = 1, q = 1;
    while (1) {
      //puts("hehe");
      while (a[p] * k < a[q]) {
        //printf("p: %d, q: %d\n", p, q);
        ++p;
      }
      pre[q] = p-1;
      mx[q] = a[q] + mx[p-1];
      a[q+1] = mx[q] + 1;
      //printf("mx[q]: %d\n", mx[q]);
      if (mx[q] >= n) break;
      q++;
    }
    //for (int i = 0; i <= q; i++) {
    //  printf("i: %d, a[i]: %d, mx[i]: %d, pre[i]: %d\n", i, a[i], mx[i], pre[i]);
    //}
    printf("Case %d: ", ii);
    if (a[q] == n) puts("lose");
    else {
      while (n != a[q]) {
        //printf("n: %d, a[q]: %d\n", n, a[q]);
        if (n > a[q]) {
          n -= a[q]; q = pre[q];
        } else {
          --q;
        }
      }
      printf("%d\n", n);
    }
  }
  return 0;
}