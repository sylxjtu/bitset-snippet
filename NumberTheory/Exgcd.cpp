tuple<long long, long long, long long> exgcd(long long x, long long y) {
  // Make sure x >= y
  if(y == 0) {
    return make_tuple(1, 0, x);
  }
  auto t = exgcd(y, x % y);
  long long a, b, g;
  tie(a, b, g) = t;
  return make_tuple(b, a - (x / y) * b, g);
}
