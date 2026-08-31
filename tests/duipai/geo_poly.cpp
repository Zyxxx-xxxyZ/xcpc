#include "geo_handbook.hpp"
// 多边形面积、周长、质心、点在多边形、多边形切割 vs 暴力。
mt19937 rng(303);
int fail = 0;
void oops(const string &s) {
	if (fail < 12) cerr << "geo_poly FAIL " << s << "\n";
	++fail;
}
int irnd(int a, int b) { return uniform_int_distribution<int>(a, b)(rng); }

vp convex_from_rand(int n, int B) {
	vp v;
	set<pair<int, int>> s;
	for (int i = 0; i < n * 4 && (int)v.size() < n; ++i) {
		int x = irnd(-B, B), y = irnd(-B, B);
		if (s.insert({x, y}).second) v.push_back({(LD)x, (LD)y});
	}
	auto h = convex_hull(v);
	return h;
}

int winding(P u, cvp p) {
	// 独立：角度和
	LD ang = 0;
	int n = (int)p.size();
	for (int i = 0; i < n; ++i) {
		P a = p[i] - u, b = p[(i + 1) % n] - u;
		ang += atan2l((a ^ b), (a * b));
	}
	if (fabsl(ang) < 0.5L) return -1;
	return 1;
}

bool on_boundary(P u, cvp p) {
	int n = (int)p.size();
	for (int i = 0; i < n; ++i)
		if (point_on_segment(u, {p[i], p[(i + 1) % n]})) return true;
	return false;
}

int brute_pip(P u, cvp p) {
	if (on_boundary(u, p)) return 0;
	return winding(u, p);
}

P brute_centroid_triangle(P a, P b, P c) {
	return (a + b + c) / 3;
}

int main() {
	for (int t = 0; t < 10000; ++t) {
		auto h = convex_from_rand(3 + t % 6, 8);
		if (h.size() < 3) continue;
		LD a2 = 0;
		int n = (int)h.size();
		for (int i = 0; i < n; ++i) a2 += h[i].x * h[(i + 1) % n].y - h[i].y * h[(i + 1) % n].x;
		if (fabsl(polygon_signed_area2(h) - a2) > 1e-8L) oops("area2");
		if (fabsl(polygon_area(h) - fabsl(a2) / 2) > 1e-8L) oops("area");
		LD peri = 0;
		for (int i = 0; i < n; ++i) peri += (h[i] - h[(i + 1) % n]).len();
		if (fabsl(polygon_perimeter(h) - peri) > 1e-8L) oops("peri");

		if (n == 3) {
			P g = polygon_centroid(h);
			P bf = brute_centroid_triangle(h[0], h[1], h[2]);
			if (fabsl(g.x - bf.x) + fabsl(g.y - bf.y) > 1e-7L) oops("centroid3");
		}

		P q{(LD)irnd(-10, 10), (LD)irnd(-10, 10)};
		int gp = point_in_polygon(q, h);
		int bp = brute_pip(q, h);
		if (gp != bp) oops("pip");

		// 用一条直线切开，结果应在直线左侧且面积不增
		P s{(LD)irnd(-6, 6), (LD)irnd(-6, 6)};
		P tt{(LD)irnd(-6, 6), (LD)irnd(-6, 6)};
		if (s != tt) {
			auto cut = polygon_cut(h, {s, tt});
			for (auto p : cut) if (turn(s, tt, p) < 0) oops("cut_side");
			if (polygon_area(cut) > polygon_area(h) + 1e-6L) oops("cut_area");
		}
		if (fail > 40) break;
	}
	int sf = fail;
	for (int t = 0; t < 1000; ++t) {
		auto h = convex_from_rand(8 + t % 8, 25);
		if (h.size() < 3) continue;
		for (int k = 0; k < 8; ++k) {
			P q{(LD)irnd(-30, 30), (LD)irnd(-30, 30)};
			if (point_in_polygon(q, h) != brute_pip(q, h)) oops("pip_large");
		}
		P s{(LD)irnd(-20, 20), (LD)irnd(-20, 20)};
		P tt = s + P{(LD)irnd(-5, 5), (LD)irnd(-5, 5)};
		if (s != tt) {
			auto cut = polygon_cut(h, {s, tt});
			for (auto p : cut) if (turn(s, tt, p) < 0) oops("cut_side_large");
		}
		if (fail > 80) break;
	}
	cout << "geo_poly small_fail=" << sf << " total_fail=" << fail << (fail ? " FAIL\n" : " OK\n");
	return fail ? 1 : 0;
}
