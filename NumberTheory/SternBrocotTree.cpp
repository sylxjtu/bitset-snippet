#include <bits/stdc++.h>
using namespace std;

// Stern–Brocot Tree求x^(2/3)的最佳有理数逼近

long long a, b, c, d;
const long double eps = 1e-15;

#define cube(x) ((x) * (x) * (x))

void solve(int k) {
  // a / b < c / d
  a = 0; b = 1; c = 1; d = 0;
  long double v = 1.0 * k * k;
  while(1) {
    long long ma = a + c, mb = b + d;
    if(mb > 100000) break;
    long double v1 = (long double) ma / mb;
    v1 = cube(v1);
    if(v1 > v) {
      c = ma;
      d = mb;
    } else {
      a = ma;
      b = mb;
    }
    if(abs(v1 - v) < eps) break;
  }
  long double v1 = (long double) a / b;
  long double v2 = (long double) c / d;
  if(8 * v < cube(v1 + v2)) {
    printf("%lld/%lld\n", a, b);
  } else {
    printf("%lld/%lld\n", c, d);
  }
}

int main() {
  int n;
  scanf("%d", &n);
  while(n--) {
    long long k;
    scanf("%lld", &k);
    solve(k);
  }
  return 0;
}
