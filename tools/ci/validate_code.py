#!/usr/bin/env python3
"""Validate primary C++ snippets and Python sources for CI."""

from __future__ import annotations

import argparse
import os
from pathlib import Path
import py_compile
import re
import subprocess
import sys
import tempfile


ROOT = Path(__file__).resolve().parents[2]
SOURCE_ROOT = ROOT / "src" / "sections"
CPP_MARKER = re.compile(r"//\s*@requires:\s*c\+\+(17|20|23)\s*$")
PY_MARKER = re.compile(r"#\s*@requires:\s*python(3\.(?:10|11|12|13|14))\s*$")
LOCAL_INCLUDE = re.compile(r'^\s*#\s*include\s*"([^"]+)"')


def first_line(path: Path) -> str:
    with path.open("r", encoding="utf-8-sig") as source:
        return source.readline().rstrip("\r\n")


def check_marker_position(path: Path) -> None:
    text = path.read_text(encoding="utf-8-sig")
    lines = text.splitlines()
    for number, line in enumerate(lines[1:], start=2):
        if "@requires:" in line:
            raise ValueError(
                f"{path.relative_to(ROOT)}:{number}: @requires 标记必须位于第一行"
            )


def python_requirement(path: Path) -> str:
    check_marker_position(path)
    line = first_line(path)
    if "@requires:" not in line:
        return "3.10"
    match = PY_MARKER.fullmatch(line.strip())
    if not match:
        raise ValueError(f"{path.relative_to(ROOT)}: Python 版本标记格式或版本不受支持")
    return match.group(1)


def cpp_requirement(path: Path) -> int:
    check_marker_position(path)
    line = first_line(path)
    if "@requires:" not in line:
        return 17
    match = CPP_MARKER.fullmatch(line.strip())
    if not match:
        raise ValueError(f"{path.relative_to(ROOT)}: C++ 版本标记格式或版本不受支持")
    return int(match.group(1))


def validate_python(target: str) -> None:
    actual = f"{sys.version_info.major}.{sys.version_info.minor}"
    if actual != target:
        raise RuntimeError(f"请求 Python {target}，当前解释器是 {actual}")

    selected = []
    for path in sorted(SOURCE_ROOT.rglob("*.py")):
        if python_requirement(path) == target:
            selected.append(path)
            py_compile.compile(str(path), doraise=True)
    print(f"Python {target}: validated {len(selected)} file(s)")


def included_sources(unit: Path) -> set[Path]:
    found: set[Path] = set()

    def visit(path: Path) -> None:
        path = path.resolve()
        if path in found:
            return
        found.add(path)
        for line in path.read_text(encoding="utf-8-sig").splitlines():
            match = LOCAL_INCLUDE.match(line)
            if not match:
                continue
            included = (path.parent / match.group(1)).resolve()
            if included.is_file() and included.suffix in {".cpp", ".cc", ".cxx", ".h", ".hpp"}:
                visit(included)

    visit(unit)
    return found


def git_output(*args: str) -> str | None:
    result = subprocess.run(
        ["git", *args],
        cwd=ROOT,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.DEVNULL,
    )
    if result.returncode != 0:
        return None
    return result.stdout.strip()


def diff_base(base: str | None) -> str | None:
    if base and set(base) != {"0"}:
        exists = git_output("cat-file", "-e", f"{base}^{{commit}}") is not None
        if exists and git_output("merge-base", "HEAD", base):
            return base

    parent = git_output("rev-parse", "--verify", "HEAD^")
    fallback = git_output("merge-base", "HEAD", parent) if parent else None
    if fallback:
        print(
            f"warning: base SHA {base or '<empty>'} is unavailable; using merge-base {fallback}",
            file=sys.stderr,
        )
        return fallback

    print(
        f"warning: base SHA {base or '<empty>'} is unavailable; skipping standalone C++ snippet compile",
        file=sys.stderr,
    )
    return None


def changed_cpp(base: str | None) -> set[Path]:
    base = diff_base(base)
    if base is None:
        return set()
    result = subprocess.run(
        ["git", "diff", "--name-only", "--diff-filter=ACMR", f"{base}...HEAD", "--", "src/sections"],
        cwd=ROOT,
        check=True,
        text=True,
        stdout=subprocess.PIPE,
    )
    return {
        (ROOT / name).resolve()
        for name in result.stdout.splitlines()
        if Path(name).suffix in {".cpp", ".cc", ".cxx"}
    }


def validate_cpp(base: str | None) -> None:
    units = sorted((ROOT / "tools").glob("test_*.cpp"))
    covered: set[Path] = set()
    for path in SOURCE_ROOT.rglob("*.cpp"):
        cpp_requirement(path)

    with tempfile.TemporaryDirectory(prefix="xcpc-ci-") as output_dir:
        for unit in units:
            sources = included_sources(unit)
            covered.update(sources)
            standard = max(cpp_requirement(path) for path in sources if path.suffix == ".cpp")
            output = Path(output_dir) / unit.stem
            command = [
                os.environ.get("CXX", "g++"),
                f"-std=c++{standard}",
                "-Wall",
                "-Wextra",
                "-Wshadow",
                "-Werror",
                str(unit),
                "-o",
                str(output),
            ]
            print("+", " ".join(command))
            subprocess.run(command, cwd=ROOT, check=True)
            subprocess.run([str(output)], cwd=ROOT, check=True)

        changed = changed_cpp(base)
        src_root = SOURCE_ROOT.resolve()
        for path in changed:
            try:
                path.relative_to(src_root)
            except ValueError:
                continue
            # 手册 src/sections 下的片段即使带 demo main() 也不是完整翻译单元
            # （依赖 bits/stdc++.h / 全局常量），不单独编译。
            covered.add(path)
        standalone = {
            path
            for path in changed
            if path not in covered
            and re.search(r"\bmain\s*\(", path.read_text(encoding="utf-8-sig"))
        }
        for path in sorted(standalone):
            standard = cpp_requirement(path)
            output = Path(output_dir) / f"standalone-{len(covered)}"
            subprocess.run(
                [
                    os.environ.get("CXX", "g++"),
                    f"-std=c++{standard}",
                    "-Wall",
                    "-Wextra",
                    "-Wshadow",
                    "-Werror",
                    str(path),
                    "-o",
                    str(output),
                ],
                cwd=ROOT,
                check=True,
            )
            covered.add(path)

    uncovered = sorted(changed - covered)
    if uncovered:
        names = "\n".join(f"  - {path.relative_to(ROOT)}" for path in uncovered)
        raise RuntimeError("以下新增/修改的 C++ 文件未纳入 tools/test_*.cpp：\n" + names)
    print(f"C++: validated {len(units)} test unit(s); covered {len(covered)} source file(s)")


def main() -> int:
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(dest="command", required=True)
    py_parser = subparsers.add_parser("python")
    py_parser.add_argument("--target", required=True)
    cpp_parser = subparsers.add_parser("cpp")
    cpp_parser.add_argument("--base")
    args = parser.parse_args()
    try:
        if args.command == "python":
            validate_python(args.target)
        else:
            validate_cpp(args.base)
    except (OSError, ValueError, RuntimeError, subprocess.CalledProcessError, py_compile.PyCompileError) as error:
        print(f"error: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
