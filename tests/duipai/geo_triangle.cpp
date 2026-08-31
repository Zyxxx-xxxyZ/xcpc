#include "geo_handbook.hpp"
// 内心、外心、垂心、费马点 vs 定义。
mt19937 rng(307);
int fail = 0;
void oops(const string &s) {
	if (fail < 12) cerr << "geo_triangle FAIL " << s << "\n";
	++fail;
}
int irnd(int a, int b) { return uniform_int_distribution<int>(a, b)(rng); }
P ip(int B) { return {(LD)irnd(-B, B), (LD)irnd(-B, B)}; }

LD ang(P o, P a, P b) {
	return rad(a - o, b - o);
}

int main() {
	for (int t = 0; t < 10000; ++t) {
		P a = ip(8), b = ip(8), c = ip(8);
		if (!turn(a, b, c)) continue;
		P I = incenter(a, b, c);
		if (fabsl(ang(I, a, b) - ang(I, a, c)) > 1e-6L &&
			fabsl(point_to_line(I, {a, b}) - point_to_line(I, {a, c})) > 1e-6L)
			oops("incenter_dist");
		LD d1 = point_to_line(I, {a, b});
		LD d2 = point_to_line(I, {b, c});
		LD d3 = point_to_line(I, {c, a});
		if (fabsl(d1 - d2) + fabsl(d2 - d3) > 1e-6L) oops("incenter");

		P O = circumcenter(a, b, c);
		if (fabsl((O - a).len() - (O - b).len()) > 1e-6L) oops("circum_ab");
		if (fabsl((O - a).len() - (O - c).len()) > 1e-6L) oops("circum_ac");

		P H = orthocenter(a, b, c);
		if (fabsl((H - a) * (c - b)) > 1e-4L) oops("ortho_a");
		if (fabsl((H - b) * (c - a)) > 1e-4L) oops("ortho_b");
		if (fabsl((H - c) * (b - a)) > 1e-4L) oops("ortho_c");

		P F = fermat_point(a, b, c);
		auto tot = [&](P x) { return (x - a).len() + (x - b).len() + (x - c).len(); };
		LD tf = tot(F);
		// 邻域扰动应不更优
		P dlt[] = {{0.01L, 0}, {-0.01L, 0}, {0, 0.01L}, {0, -0.01L}, {0.01L, 0.01L}, {-0.01L, 0.01L}};
		for (auto d : dlt)
			if (tot(F + d) + 1e-8L < tf) oops("fermat_local");
		if (fail > 40) break;
	}
	int sf = fail;
	for (int t = 0; t < 1000; ++t) {
		P a = ip(40), b = ip(40), c = ip(40);
		if (!turn(a, b, c)) continue;
		P I = incenter(a, b, c);
		LD d1 = point_to_line(I, {a, b}), d2 = point_to_line(I, {b, c}), d3 = point_to_line(I, {c, a});
		if (fabsl(d1 - d2) + fabsl(d2 - d3) > 1e-5L) oops("incenter_large");
		P O = circumcenter(a, b, c);
		if (fabsl((O - a).len() - (O - b).len()) + fabsl((O - a).len() - (O - c).len()) > 1e-5L)
			oops("circum_large");
		P H = orthocenter(a, b, c);
		if (fabsl((H - a) * (c - b)) > 1e-3L) oops("ortho_large");
		P F = fermat_point(a, b, c);
		auto tot = [&](P x) { return (x - a).len() + (x - b).len() + (x - c).len(); };
		P dlt[] = {{0.05L, 0}, {-0.05L, 0}, {0, 0.05L}, {0, -0.05L}};
		LD tf = tot(F);
		for (auto d : dlt)
			if (tot(F + d) + 1e-6L < tf) oops("fermat_large");
		if (fail > 80) break;
	}
	cout << "geo_triangle small_fail=" << sf << " total_fail=" << fail << (fail ? " FAIL\n" : " OK\n");
	return fail ? 1 : 0;
}
