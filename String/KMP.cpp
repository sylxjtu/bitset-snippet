/*
 * @title: KMP 字符串匹配
 * @description: KMP 字符串匹配算法
 * @arguments:
 *  S: 源串的首指针
 *  P: 模式串的首指针
 *  N: Next 数组 [alloc only]
 * @performance:
 *  Time: O(|P| + |S|)
 *  Space: O(|S| + |P| + |N|)
 *    Extra: O(|N|)
 * @dependence:
 *  Matched(int index);
 * @note: S 与 P 的终止符为 '\0'
 */

void KMPInit(char *P, int *N) {
  int k = N[0] = 0;
  for (int i = 1; P[i] != '\0'; i++) {
    while (k && P[k] != P[i]) {
      k = N[k - 1];
    }
    if (P[k] == P[i]) {
      k++;
    }
    N[i] = k;
  }
}

void KMP(char *S, char *P, int *N) {
  int k = 0;
  for (int i = 0; S[i] != '\0'; i++) {
    while (k && P[k] != S[i]) {
      k = N[k - 1];
    }
    if (P[k] == S[i]) {
      k++;
    }
    if (P[k] == '\0') {
      Matched(i);
      k = N[k - 1];
    }
  }
}
