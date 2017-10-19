int div_down(int n, int d) {
  if (n < 0) {
    return -((d - n - 1) / d);
  } else {
    return n / d;
  }
}

int div_up(int n, int d) {
  if (n < 0) {
    return -(-n / d);
  } else {
    return (n + d - 1) / d;
  }
}

int div_to_zero(int n, int d) {
  return n / d;
}

int div_to_nearest(int n, int d) {
  if (n < 0) {
    return (n - d/2 + 1) / d;
  } else {
    return (n + d/2) / d;
  }
}
