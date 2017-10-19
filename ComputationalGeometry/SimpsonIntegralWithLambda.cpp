// 自适应辛普森积分
// 依赖
//   double simpeps
//     误差限
// 参数
//   l 下界
//   h 上界
//   T f(T) 被积函数

#include <bits/stdc++.h>
using namespace std;
const double simpeps = 1e-8;

template<class T>
inline T simpson(T l, T h, function<T(T)> f){
  return (h - l) * (f(l) + 4 * f((l + h) / 2) + f(h)) / 6;
}

template<class T>
inline T adaptsimp(T l, T h, function<T(T)> f){
  T a = simpson(l, (l + h) / 2, f);
  T b = simpson((l + h) / 2, h, f);
  T c = simpson(l, h, f);
  if(fabs(a + b - c) > 15 * simpeps){
    a = adaptsimp(l, (l + h) / 2, f);
    b = adaptsimp((l + h) / 2, h, f);
  }
  return a + b;
}

// 测试代码

int main() {
  cout << adaptsimp<double>(1, 2, [](double x){return x;}) << endl;
}
