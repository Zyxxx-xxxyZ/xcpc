# 手册暴力对拍（不进入 CI）

本目录是手册的**本地**对拍程序：

- `dp_*.cpp` / `dp_formulas.py`：B1–B55 活页对拍
- `geo_*.cpp`：计算几何活页对暴力 / 独立实现（小数据 ≥10000 组，大数据 ≥1000 组）

几何对拍找出的缺陷见 `geometry-issues.md`。`geo_ray` 等条目会因手册缺陷失败，这是预期结果而不是测试写错。

**GitHub Actions 不会编译或运行这里的任何文件。** CI 的 C++ 任务只处理 `src/sections` 与 `tools/test_*.cpp`；Python 任务只扫描 `src/sections/**/*.py`。

## 运行

需要较新的 GCC（含 `bits/stdc++.h`、`__int128`），例如 Homebrew `g++-16`。

```bash
cd tests/duipai
CXX=g++-16 ./run.sh
```

只跑某一个：

```bash
g++-16 -O2 -std=c++17 dp_bsgs.cpp -o /tmp/dp_bsgs && /tmp/dp_bsgs
python3 dp_formulas.py
```

产物写在 `build/`（已 gitignore），不要提交二进制。

## 与手册的对应

程序名 `dp_xxx` / `geo_xxx` 对应活页片段。源码自包含：把手册逻辑抄进测试再和暴力比，不 `#include` 手册路径。`geo_*.cpp` 共用 `geo_handbook.hpp`（8.1–8.4 及后续片段的测试副本）。
