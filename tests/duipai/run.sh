#!/usr/bin/env bash
# 本地对拍。不要从 GitHub Actions 调用。
set -euo pipefail
cd "$(dirname "$0")"
CXX="${CXX:-g++}"
if command -v g++-16 >/dev/null 2>&1 && [[ "$CXX" == "g++" ]]; then
  CXX=g++-16
fi
mkdir -p build
fail=0
shopt -s nullglob
for src in dp_*.cpp; do
  name="${src%.cpp}"
  echo "COMPILE $name"
  if ! "$CXX" -O2 -std=c++17 -pipe "$src" -o "build/$name"; then
    echo "BUILD FAIL $name"
    fail=$((fail + 1))
    continue
  fi
  echo "RUN $name"
  if ! "build/$name"; then
    echo "RUN FAIL $name"
    fail=$((fail + 1))
  fi
done
echo "RUN dp_formulas.py"
if ! python3 dp_formulas.py; then
  echo "RUN FAIL dp_formulas.py"
  fail=$((fail + 1))
fi
if [[ "$fail" -ne 0 ]]; then
  echo "duipai failures: $fail"
  exit 1
fi
echo "duipai all OK"
