#include "geo_handbook.hpp"
// 黄金三分（手册逻辑，T 在测试中补上）、自适应 Simpson vs 已知函数。
mt19937 rng(309);
int fail = 0;
void oops(const string &s) {
	if (fail < 12) cerr << "geo_opt FAIL " << s << "\n";
	++fail;
}

// 手册 yzh/golden_ternary.cpp，仅补上未定义的 T 才能编译。
constexpr LD Rphi = (sqrt(5) - 1) / 2;
auto split = [](LD l, LD r) { return l + (r - l) * Rphi; };
const int T = 60;
LD golden_handbook(LD a, LD c, function<LD(LD)> f) {
	LD b = split(a, c), bv = f(b);
	for (int _ = T; _; _--) {
		LD x = split(a, b), xv = f(x);
		if (xv < bv) c = b, b = x, bv = xv;
		else a = c, c = x;
	}
	return bv;
}

LD golden_correct(LD a, LD c, function<LD(LD)> f) {
	auto sp = [](LD l, LD r) { return l + (r - l) * Rphi; };
	LD b = sp(a, c), bv = f(b);
	for (int _ = 80; _; _--) {
		LD x = sp(a, b), xv = f(x);
		if (xv < bv) c = b, b = x, bv = xv;
		else {
			a = x;
			x = sp(b, c);
			xv = f(x);
			if (xv < bv) a = b, b = x, bv = xv;
			else c = x;
		}
	}
	return bv;
}

LD f_quad(LD x) { return (x - 2) * (x - 2); }
LD f_sin(LD x) { return sinl(x); }
LD f_x2(LD x) { return x * x; }
LD f_exp(LD x) { return expl(-x * x); }

int main() {
	simpson S;
	for (int t = 0; t < 10000; ++t) {
		LD lo = (LD)((int)(rng() % 11) - 5);
		LD hi = lo + 1 + (rng() % 5);
		LD g = golden_handbook(lo, hi, f_quad);
		LD bf = golden_correct(lo, hi, f_quad);
		// 二次函数最小值在 clip(2, lo, hi)
		LD mid = min(hi, max(lo, (LD)2));
		LD exact = f_quad(mid);
		if (fabsl(bf - exact) > 1e-6L && fail < 3) {
			// 正确实现本身的误差，不当手册问题
		}
		if (g > exact + 0.05L) oops("golden_quad");

		LD gs = S.solve(f_x2, 0, 1, 1e-10L);
		if (fabsl(gs - (LD)1 / 3) > 1e-8L) oops("simpson_x2");
		if (fail > 40) break;
	}
	int sf = fail;
	for (int t = 0; t < 1000; ++t) {
		LD a = -3 + (t % 7) * 0.1L;
		LD c = 5 + (t % 5) * 0.2L;
		LD g = golden_handbook(a, c, f_quad);
		LD exact = f_quad(min(c, max(a, (LD)2)));
		if (g > exact + 0.02L) oops("golden_large");
		LD gs = S.solve(f_sin, 0, pi, 1e-10L);
		if (fabsl(gs - 2) > 1e-6L) oops("simpson_sin");
		if (fail > 80) break;
	}
	cout << "geo_opt small_fail=" << sf << " total_fail=" << fail << (fail ? " FAIL\n" : " OK\n");
	return fail ? 1 : 0;
}
