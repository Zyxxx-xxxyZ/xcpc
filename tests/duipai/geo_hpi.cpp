#include "geo_handbook.hpp"
// 半平面交、整数半平面 cut vs 多边形切割暴力。
mt19937 rng(305);
int fail = 0;
void oops(const string &s) {
	if (fail < 15) cerr << "geo_hpi FAIL " << s << "\n";
	++fail;
}
int irnd(int a, int b) { return uniform_int_distribution<int>(a, b)(rng); }

vp box(LD B) {
	return {{-B, -B}, {B, -B}, {B, B}, {-B, B}};
}

bool inside_hp(P p, L l) {
	return turn(l.s, l.t, p) >= 0;
}

vp brute_hpi(const vector<L> &hs, LD B = 50) {
	vp cur = box(B);
	for (auto l : hs) cur = polygon_cut(cur, l);
	return convex_hull(cur);
}

bool same_poly(vp a, vp b) {
	a = convex_hull(a); b = convex_hull(b);
	if (a.size() != b.size()) {
		if (polygon_area(a) < 1e-6L && polygon_area(b) < 1e-6L) return true;
		return fabsl(polygon_area(a) - polygon_area(b)) < 1e-4L;
	}
	if (a.empty()) return true;
	int n = (int)a.size();
	for (int s = 0; s < n; ++s) {
		bool ok = true;
		for (int i = 0; i < n; ++i)
			if (a[i] != b[(i + s) % n]) { ok = false; break; }
		if (ok) return true;
	}
	return fabsl(polygon_area(a) - polygon_area(b)) < 1e-4L;
}

vector<IL> poly_to_il(cvp p) {
	vector<IL> r;
	int n = (int)p.size();
	for (int i = 0; i < n; ++i) r.push_back(IL(p[i], p[(i + 1) % n]));
	return r;
}

vp il_to_poly(const vector<IL> &h) {
	int n = (int)h.size();
	if (n < 3) return {};
	vp r(n);
	for (int i = 0; i < n; ++i) r[i] = il_intersection(h[i], h[(i + 1) % n]);
	return r;
}

int main() {
	for (int t = 0; t < 10000; ++t) {
		int m = 1 + t % 6;
		vector<L> hs;
		for (int i = 0; i < m; ++i) {
			P s{(LD)irnd(-8, 8), (LD)irnd(-8, 8)};
			P dir{(LD)irnd(-5, 5), (LD)irnd(-5, 5)};
			if (dir.x == 0 && dir.y == 0) dir = {1, 0};
			hs.push_back({s, s + dir});
		}
		// 手册要求加大框
		vector<L> with_box = hs;
		LD B = 40;
		with_box.push_back({{-B, -B}, {B, -B}});
		with_box.push_back({{B, -B}, {B, B}});
		with_box.push_back({{B, B}, {-B, B}});
		with_box.push_back({{-B, B}, {-B, -B}});
		vp g = hpi(with_box);
		vp bf = brute_hpi(hs, B);
		if (!same_poly(g, bf)) {
			if (fabsl(polygon_area(g) - polygon_area(bf)) > 1e-3L) oops("hpi_area");
		}
		if (!g.empty()) {
			for (auto l : with_box)
				for (auto p : g)
					if (turn(l.s, l.t, p) < 0) oops("hpi_outside");
		}

		// 整数半平面 cut vs polygon_cut
		auto poly = convex_hull({
			{(LD)irnd(-6, 0), (LD)irnd(-6, 0)},
			{(LD)irnd(0, 6), (LD)irnd(-6, 0)},
			{(LD)irnd(0, 6), (LD)irnd(0, 6)},
			{(LD)irnd(-6, 0), (LD)irnd(0, 6)},
			{(LD)irnd(-4, 4), (LD)irnd(-4, 4)}
		});
		if (poly.size() >= 3) {
			auto ils = poly_to_il(poly);
			P s{(LD)irnd(-8, 8), (LD)irnd(-8, 8)};
			P dir{(LD)irnd(-4, 4), (LD)irnd(-4, 4)};
			if (dir.x == 0 && dir.y == 0) dir = {0, 1};
			IL cutl(s, s + dir);
			auto got = cut_integral_hpi(ils, cutl);
			auto bfcut = polygon_cut(poly, {s, s + dir});
			if (got.size() >= 3 && bfcut.size() >= 3) {
				auto gp = il_to_poly(got);
				if (fabsl(polygon_area(gp) - polygon_area(bfcut)) > 1e-2L)
					oops("il_cut_area");
			}
		}
		if (fail > 40) break;
	}
	int sf = fail;
	for (int t = 0; t < 1000; ++t) {
		int m = 4 + t % 8;
		vector<L> hs;
		for (int i = 0; i < m; ++i) {
			P s{(LD)irnd(-20, 20), (LD)irnd(-20, 20)};
			P dir{(LD)irnd(-10, 10), (LD)irnd(-10, 10)};
			if (dir.x == 0 && dir.y == 0) dir = {1, 1};
			hs.push_back({s, s + dir});
		}
		LD B = 80;
		vector<L> with_box = hs;
		with_box.push_back({{-B, -B}, {B, -B}});
		with_box.push_back({{B, -B}, {B, B}});
		with_box.push_back({{B, B}, {-B, B}});
		with_box.push_back({{-B, B}, {-B, -B}});
		vp g = hpi(with_box);
		vp bf = brute_hpi(hs, B);
		if (fabsl(polygon_area(g) - polygon_area(bf)) > 1e-2L) oops("hpi_large");
		if (fail > 80) break;
	}
	cout << "geo_hpi small_fail=" << sf << " total_fail=" << fail << (fail ? " FAIL\n" : " OK\n");
	return fail ? 1 : 0;
}
