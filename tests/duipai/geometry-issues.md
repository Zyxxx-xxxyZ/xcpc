# 计算几何对拍发现的 issue

对拍条件：小数据 ≥10000 组，大数据 ≥1000 组；手册逻辑抄进测试，不 `#include` 手册路径。
编译：Homebrew `g++-16`，`-std=c++17 -O2`。运行：`cd tests/duipai && CXX=g++-16 ./run.sh`。

对拍结果（`g++-16 -O2 -std=c++17`，本机）：

| 程序 | 结果 |
| --- | --- |
| `geo_prim` / `geo_hull` / `geo_poly` / `geo_hpi` / `geo_query` / `geo_circle` / `geo_area` / `geo_mesh` / `geo_metric` / `geo_opt` / `geo_triangle` / `geo_3d` / `geo_extra` | OK（整数小坐标暴力一致） |
| `geo_eq` | **FAIL** `both_lt_and_eq=11000`（G1，每组都中） |
| `geo_ray` | **FAIL** 10967 组非平行射线中 **FN=308 FP=0**（G2） |
| `geo_delaunay_lots` | **FAIL** `[-1e7,1e7]` 1000 组有 996 个点在超三角形外（G4） |

下面每一条都可以单独保存为 `bug.cpp` 后编译运行。文件位置按当前手册活页。

---

## G1. `P::operator<` 用精确比较，`operator==` 用 eps，二者可同时成立

位置：`src/sections/ComputationalGeometry/assets/Geometry/8.2-point-vector.cpp`

`sort` / `set<P>` 走 `<`（精确），`unique` 走 `==`（eps）。两点在 eps 内既 `a==b` 又 `a<b`。

```cpp
#include <bits/stdc++.h>
using namespace std;
using LD = long double;
const LD eps = 1e-12L;
int sgn(LD x) { return x > eps ? 1 : (x < -eps ? -1 : 0); }
struct P { LD x, y; };
bool operator<(const P& a, const P& b) { return a.x == b.x ? a.y < b.y : a.x < b.x; }
bool operator==(const P& a, const P& b) { return !sgn(a.x - b.x) && !sgn(a.y - b.y); }
int main() {
  P a{0, 0}, b{1e-13L, 0};
  cout << boolalpha << "a<b " << (a < b) << " a==b " << (a == b) << "\n";
  // 期望：等价关系与序关系一致。实际：两者同时为 true。
}
```

---

## G2. `ll_intersection` 的交点通不过同一对直线的 `point_on_line`（eps=1e-12）

位置：`src/sections/ComputationalGeometry/assets/Geometry/8.3-line.cpp`

整数坐标、方向叉积达数千时，重建交点的叉积残差常略大于 `1e-12`，`sgn` 判不在直线上。`ray_intersection_judge` 用「先求交再 `point_on_ray`」，因此对真实相交的射线出现假阴性。

在 `[-200,200]` 整数端点、非平行射线的精确有理参数对拍中，假阴性约 30%。

```cpp
#include <bits/stdc++.h>
using namespace std;
using LD = long double;
const LD eps = 1e-12L;
int sgn(LD x) { return x > eps ? 1 : (x < -eps ? -1 : 0); }
struct P { LD x, y; };
using cp = const P&;
P operator-(cp a, cp b) { return {a.x - b.x, a.y - b.y}; }
P operator*(cp a, LD k) { return {a.x * k, a.y * k}; }
LD operator^(cp a, cp b) { return a.x * b.y - a.y * b.x; }
struct L { P s, t; };
P ll_intersection(const L& a, const L& b) {
  LD s1 = (a.t - a.s) ^ (b.s - a.s), s2 = (a.t - a.s) ^ (b.t - a.s);
  return {(b.s.x * s2 - b.t.x * s1) / (s2 - s1), (b.s.y * s2 - b.t.y * s1) / (s2 - s1)};
}
bool point_on_line(cp p, const L& l) { return !sgn((p - l.s) ^ (l.t - l.s)); }
int main() {
  L a{{62, -31}, {-79, -75}}, b{{-45, 14}, {28, 60}};
  P p = ll_intersection(a, b);
  LD cr = (p - a.s) ^ (a.t - a.s);
  cout << "on_line=" << point_on_line(p, a) << " residual=" << (double)cr << "\n";
  // on_line=0，residual≈1.14e-12 > eps
}
```

`geo_ray` 对拍里的一条假阴性（t=262/12>0，s=800/12>0，两射线必交；手册 `judge=false`）：

```cpp
#include <bits/stdc++.h>
using namespace std;
using LD = long double;
const LD eps = 1e-12L;
int sgn(LD x) { return x > eps ? 1 : (x < -eps ? -1 : 0); }
struct P { LD x, y; };
using cp = const P&;
P operator-(cp a, cp b) { return {a.x - b.x, a.y - b.y}; }
P operator*(cp a, LD k) { return {a.x * k, a.y * k}; }
LD operator*(cp a, cp b) { return a.x * b.x + a.y * b.y; }
LD operator^(cp a, cp b) { return a.x * b.y - a.y * b.x; }
bool operator==(cp a, cp b) { return !sgn(a.x - b.x) && !sgn(a.y - b.y); }
struct L { P s, t; };
bool point_on_line(cp a, const L& l) { return !sgn((a - l.s) ^ (l.t - l.s)); }
bool point_on_ray(cp a, const L& b) {
  if (b.s == b.t) return a == b.s;
  return point_on_line(a, b) && sgn((a - b.s) * (b.t - b.s)) >= 0;
}
P ll_intersection(const L& a, const L& b) {
  LD s1 = (a.t - a.s) ^ (b.s - a.s), s2 = (a.t - a.s) ^ (b.t - a.s);
  return {(b.s.x * s2 - b.t.x * s1) / (s2 - s1), (b.s.y * s2 - b.t.y * s1) / (s2 - s1)};
}
bool ray_intersection_judge(const L& a, const L& b) {
  P p = ll_intersection(a, b);
  return point_on_ray(p, a) && point_on_ray(p, b);
}
int main() {
  L u{{25, -5}, {-7, 29}}, v{{-7, 4}, {-17, 15}};
  P p = ll_intersection(u, v);
  cout << boolalpha << "judge=" << ray_intersection_judge(u, v)
       << " residual=" << (double)((p - u.s) ^ (u.t - u.s)) << "\n";
  // judge=false  residual≈2.5e-12
}
```

---

## G3. 黄金三分：`T` 未定义，`auto` 形参需要 C++20

位置：`src/sections/ComputationalGeometry/assets/yzh/golden_ternary.cpp`  
`index.tex` 高亮第 7 行。

```cpp
#include <bits/stdc++.h>
using namespace std;
using LD = long double;
constexpr LD R = (sqrt(5) - 1) / 2;
auto split = [](LD l, LD r) { return l + (r - l) * R; };
LD solve(LD a, LD c, auto f) {
  LD b = split(a, c), bv = f(b);
  for (int _ = T; _; _--) {          // T 未定义，C++17 无法编译
    LD x = split(a, b), xv = f(x);
    if (xv < bv) c = b, b = x, bv = xv;
    else a = c, c = x;
  }
  return bv;
}
int main() { cout << solve(0, 5, [](LD x) { return (x - 2) * (x - 2); }) << "\n"; }
```

`g++-16 -std=c++17`：`T was not declared`；`auto` 形参仅在 C++20/`-fconcepts` 下可用。  
补上 `T` 后，`else a=c,c=x` 对二次函数仍能收敛到最小值（区间会被翻转），逻辑未必错，但片段本身不能当 C++17 活页粘贴。

---

## G4. Delaunay 超三角形 `LOTS=1e6`，常见坐标在外部

位置：`src/sections/ComputationalGeometry/assets/Geometry/DelaunayTriangulation.cpp`

超三角形顶点 `(-LOTS,-LOTS),(+LOTS,-LOTS),(0,+LOTS)`。y=0 时三角形只覆盖 |x|≤5e5。`contains` 失败后 `find` 退回 `ch[0]`，点被插进错误三角形。

```cpp
// 手册常量
const LD LOTS = 1e6;
// 根三角形
P(-LOTS,-LOTS), P(+LOTS,-LOTS), P(0,+LOTS);
```

```cpp
#include <bits/stdc++.h>
using namespace std;
using LD = long double;
const LD eps = 1e-12L;
int sgn(LD x) { return x > eps ? 1 : (x < -eps ? -1 : 0); }
struct P { LD x, y; };
LD operator^(P a, P b) { return a.x * b.y - a.y * b.x; }
P operator-(P a, P b) { return {a.x - b.x, a.y - b.y}; }
LD dir(P a, P b, P p) { return (b - a) ^ (p - a); }
bool contains(P a, P b, P c, P q) {
  return sgn(dir(a, b, q)) >= 0 && sgn(dir(b, c, q)) >= 0 && sgn(dir(c, a, q)) >= 0;
}
int main() {
  const LD LOTS = 1e6;
  P A{-LOTS, -LOTS}, B{+LOTS, -LOTS}, C{0, +LOTS};
  cout << "contains (0,0)     " << contains(A, B, C, {0, 0}) << "\n";
  cout << "contains (1e6,0)   " << contains(A, B, C, {1e6L, 0}) << "\n";
  cout << "contains (1e7,0)   " << contains(A, B, C, {1e7L, 0}) << "\n";
  cout << "contains (0,1e7)   " << contains(A, B, C, {0, 1e7L}) << "\n";
  // 后三个均为 0。对 {(0,0),(1e7,0),(0,1e7)} 建图后，叶三角形仍绑在超三角形顶点上。
}
```

---

## G5. 三点外接圆构造在共线时产生 NaN

位置：`src/sections/ComputationalGeometry/assets/Geometry/8.4-circle.cpp`  
注释写了「三点不能共线」，没有守卫。直接调用 `C(a,b,c)` 时 `p^q==0` 除零。

```cpp
#include <bits/stdc++.h>
using namespace std;
using LD = long double;
struct P { LD x, y; LD len2() const { return x * x + y * y; } };
P operator-(P a, P b) { return {a.x - b.x, a.y - b.y}; }
P operator+(P a, P b) { return {a.x + b.x, a.y + b.y}; }
P operator/(P a, LD k) { return {a.x / k, a.y / k}; }
LD operator^(P a, P b) { return a.x * b.y - a.y * b.x; }
int main() {
  P x{0, 0}, y{1, 0}, z{2, 0};
  P p = y - x, q = z - x;
  P s{p.len2() / 2, q.len2() / 2};
  LD d = p ^ q;
  P c = x + P{s.x * q.y - s.y * p.y, p.x * s.y - p.y * s.x} / d;
  cout << "d=" << d << " cx=" << c.x << " cy=" << c.y
       << " finite=" << isfinite((double)c.x) << "\n";
}
```

随机增量最小圆覆盖在全共线点集上通常不会走到这个构造（先被直径圆盖住），但构造函数本身不安全。

---

## G6. 凸包注释声称按 `(y,x)` 排序，实际 `operator<` 是 `(x,y)`

位置：`src/sections/ComputationalGeometry/assets/Geometry/凸包.cpp`

```cpp
sort(a.begin(), a.end()); // 小于号 (y, x) 字典序
// ...
return ret; } // 小于号为 (y,x) 时边 [0, 2pi) 逆时针
```

```cpp
#include <bits/stdc++.h>
using namespace std;
struct P { long double x, y; };
bool operator<(P a, P b) { return a.x == b.x ? a.y < b.y : a.x < b.x; }
int main() {
  vector<P> a{{2, 0}, {1, 1}, {0, 0}, {1, -1}};
  sort(a.begin(), a.end());
  cout << "first after sort (" << a[0].x << "," << a[0].y << ")\n";
  // 实际起点 (0,0)（最左）。若按注释 (y,x) 应先看到 (1,-1)（最下）。
  // Andrew 用 (x,y) 仍然得到逆时针凸包，注释是错的，算法对 (x,y) 可用。
}
```

对拍 10000 组小凸包 / 1000 组大凸包与独立单调链一致，起点均为 `min (x,y)`。

---

## G7. `inv_c2c` 在反演中心位于圆内时半径为负

位置：`src/sections/ComputationalGeometry/assets/yzh/circle_inversion.cpp`  
注释要求「点 O 在圆 A 外」，没有检查。

```cpp
#include <bits/stdc++.h>
using namespace std;
using LD = long double;
struct P { LD x, y; LD len() const { return sqrtl(x * x + y * y); } };
P operator-(P a, P b) { return {a.x - b.x, a.y - b.y}; }
P operator+(P a, P b) { return {a.x + b.x, a.y + b.y}; }
P operator*(P a, LD k) { return {a.x * k, a.y * k}; }
int main() {
  P O{0.5, 0}; P Ac{0, 0}; LD Ar = 2, R = 1;
  LD OA = (Ac - O).len();
  LD RB = 0.5 * R * R * (1 / (OA - Ar) - 1 / (OA + Ar));
  cout << "RB=" << RB << "\n"; // 负半径
}
```

---

## G8. 自适应 Simpson 用 `abs` 而不是 `fabsl`

位置：`src/sections/ComputationalGeometry/assets/Math/Simpson.cpp`

```cpp
if (abs (left + right - a) <= 15 * eps)
```

配 `bits/stdc++.h` 时 `std::abs(long double)` 存在，对拍 `[0,1]` 上 `x^2`、`[0,π]` 上 `sin` 与解析值一致。活页没有 include；若只看见 C 的 `abs(int)`：

```cpp
#include <cstdlib>
#include <cstdio>
int main() {
  long double err = 0.3L;
  std::printf("abs(0.3L) via cstdlib = %d\n", abs(err)); // 截成 0
}
```

---

## G9. 圆并 / Delaunay 依赖片段外的全局符号

- `圆并.cpp`：`MAXN`、`circles[]`、`coverage_area[]` 未定义。
- `DelaunayTriangulation.cpp`：`N`、`MAX_TRIS`、全局 `triange_pool`。

粘贴后不能单独编译。对拍里要自行 `#define MAXN` / 缩小 `N`。

---

## G10. `Formula(Saga).tex` 外心公式按字面（不把 `AB^T` 当旋 90°）是错的

位置：`src/sections/ComputationalGeometry/assets/Geometry/Formula(Saga).tex`

```
\vec{O} = \frac{\vec{A}+\vec{B}-\frac{\overrightarrow{BC}\cdot\overrightarrow{CA}}{\overrightarrow{AB}\times\overrightarrow{BC}}\overrightarrow{AB}^T}{2}
```

三角形 `(1,1),(5,2),(2,6)`：把 `AB^T` 当 `AB` 得到的点不是外心；当 `rot90(AB)` 则与 `C(a,b,c).c` 一致。记号有歧义。

---

## 对拍里目前与暴力一致的部分（整数小坐标）

凸包、闵可夫斯基和、旋转卡壳直径/宽/最小面积矩形、多边形面积/PIP/切割、半平面交（加框）、ConvexQuery、动态上凸包、ear clipping（凸包输入）、圆交面积、多边形∩圆（若干精确算例）、最近点对、球面距离、三角形四心、三维旋转/凸包/最小球、圆上整点 r≤200 与 5-12-13 倍式。

这些不能说明大坐标或病态输入也安全（见 G2、G4）。
