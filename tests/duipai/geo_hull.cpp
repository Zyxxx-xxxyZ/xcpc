#include "geo_handbook.hpp"
// Andrew 凸包、闵可夫斯基和、旋转卡壳 vs 暴力。
mt19937 rng(302);
int fail = 0;
void oops(const string &s) {
	if (fail < 12) cerr << "geo_hull FAIL " << s << "\n";
	++fail;
}
int irnd(int a, int b) { return uniform_int_distribution<int>(a, b)(rng); }

vp brute_hull(vp a) {
	sort(a.begin(), a.end());
	a.erase(unique(a.begin(), a.end(), [](P u, P v) {
		return u.x == v.x && u.y == v.y;
	}), a.end());
	int n = (int)a.size();
	if (n <= 1) return a;
	auto crossll = [](P o, P a, P b) {
		return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
	};
	vp lo, up;
	for (auto p : a) {
		while (lo.size() >= 2 && crossll(lo[lo.size() - 2], lo.back(), p) <= 0) lo.pop_back();
		lo.push_back(p);
	}
	for (int i = n - 1; i >= 0; --i) {
		auto p = a[i];
		while (up.size() >= 2 && crossll(up[up.size() - 2], up.back(), p) <= 0) up.pop_back();
		up.push_back(p);
	}
	lo.pop_back();
	up.pop_back();
	lo.insert(lo.end(), up.begin(), up.end());
	return lo;
}

bool hull_eq(vp a, vp b) {
	if (a.size() != b.size()) return false;
	if (a.empty()) return true;
	auto rot = [](vp v) {
		int k = 0;
		for (int i = 1; i < (int)v.size(); ++i) if (v[i] < v[k]) k = i;
		rotate(v.begin(), v.begin() + k, v.end());
		return v;
	};
	a = rot(a); b = rot(b);
	for (size_t i = 0; i < a.size(); ++i)
		if (a[i].x != b[i].x || a[i].y != b[i].y) return false;
	return true;
}

bool pt_in_hull(cvp h, P p) {
	int n = (int)h.size();
	if (!n) return false;
	if (n == 1) return p.x == h[0].x && p.y == h[0].y;
	if (n == 2) return point_on_segment(p, {h[0], h[1]});
	for (int i = 0; i < n; ++i)
		if (turn(h[i], h[(i + 1) % n], p) < 0) return false;
	return true;
}

LD brute_diam(cvp p) {
	LD ans = 0;
	for (size_t i = 0; i < p.size(); ++i)
		for (size_t j = i + 1; j < p.size(); ++j)
			ans = max(ans, (p[i] - p[j]).len());
	return ans;
}

LD brute_width(cvp p) {
	int n = (int)p.size();
	if (n < 3) return 0;
	LD ans = INF;
	for (int i = 0; i < n; ++i) {
		P a = p[i], b = p[(i + 1) % n];
		LD e = (b - a).len();
		if (!sgn(e)) continue;
		LD mx = 0;
		for (int j = 0; j < n; ++j)
			mx = max(mx, fabsl((b - a) ^ (p[j] - a)) / e);
		ans = min(ans, mx);
	}
	return ans;
}

LD brute_min_rect(cvp p) {
	int n = (int)p.size();
	if (!n) return 0;
	if (n == 1) return 0;
	LD ans = INF;
	for (int i = 0; i < n; ++i) {
		P u = (p[(i + 1) % n] - p[i]);
		if (!sgn(u.len2())) continue;
		u = u.unit();
		P v = u.rot90();
		LD min_u = INF, max_u = -INF, min_v = INF, max_v = -INF;
		for (auto q : p) {
			LD du = q * u, dv = q * v;
			min_u = min(min_u, du); max_u = max(max_u, du);
			min_v = min(min_v, dv); max_v = max(max_v, dv);
		}
		ans = min(ans, (max_u - min_u) * (max_v - min_v));
	}
	return ans;
}

vp rand_pts(int n, int B) {
	vp v;
	set<pair<int, int>> s;
	for (int i = 0; i < n * 3 && (int)v.size() < n; ++i) {
		int x = irnd(-B, B), y = irnd(-B, B);
		if (s.insert({x, y}).second) v.push_back({(LD)x, (LD)y});
	}
	if (v.empty()) v.push_back({0, 0});
	return v;
}

int main() {
	for (int t = 0; t < 10000; ++t) {
		auto pts = rand_pts(2 + t % 8, 7);
		auto g = convex_hull(pts);
		auto b = brute_hull(pts);
		if (!hull_eq(g, b)) oops("hull");
		// 注释声称 (y,x) 排序，实际 operator< 是 (x,y)：从最左点起步
		if (g.size() >= 1) {
			P mn = pts[0];
			for (auto p : pts) if (p < mn) mn = p;
			if (g[0].x != mn.x || g[0].y != mn.y) oops("hull_start_not_xmin");
		}
		if (g.size() >= 3) {
			if (polygon_signed_area2(g) < -eps) oops("hull_not_ccw");
			LD gd = convex_diameter(g);
			LD bd = brute_diam(g);
			if (fabsl(gd - bd) > 1e-7L) oops("diameter");
			LD gw = convex_min_width(g);
			LD bw = brute_width(g);
			if (fabsl(gw - bw) > 1e-6L) oops("width");
			LD gr = min_area_rectangle(g).area;
			LD br = brute_min_rect(g);
			if (fabsl(gr - br) > 1e-5L) oops("minrect");
		}
		if (fail > 40) break;
	}
	int sf = fail;
	for (int t = 0; t < 10000; ++t) {
		auto A = brute_hull(rand_pts(1 + t % 5, 6));
		auto B = brute_hull(rand_pts(1 + t % 5, 6));
		if (A.empty()) A.push_back({0, 0});
		if (B.empty()) B.push_back({0, 0});
		auto g = convex_hull(minkowski_sum(A, B));
		vp all;
		for (auto u : A) for (auto v : B) all.push_back(u + v);
		auto bf = brute_hull(all);
		bool ok = true;
		if (g.size() >= 3 && bf.size() >= 3) {
			for (auto p : g) if (!pt_in_hull(bf, p)) ok = false;
			for (auto p : bf) if (!pt_in_hull(g, p)) ok = false;
		} else {
			ok = hull_eq(g, bf);
		}
		if (!ok) oops("minkowski");
		if (fail > 60) break;
	}
	int sf2 = fail;
	for (int t = 0; t < 1000; ++t) {
		auto pts = rand_pts(12 + t % 10, 30);
		auto g = convex_hull(pts);
		auto b = brute_hull(pts);
		if (!hull_eq(g, b)) oops("hull_large");
		if (g.size() >= 3) {
			if (fabsl(convex_diameter(g) - brute_diam(g)) > 1e-6L) oops("diam_large");
			if (fabsl(convex_min_width(g) - brute_width(g)) > 1e-5L) oops("width_large");
			if (fabsl(min_area_rectangle(g).area - brute_min_rect(g)) > 1e-4L) oops("rect_large");
		}
		auto A = brute_hull(rand_pts(8, 20));
		auto B = brute_hull(rand_pts(8, 20));
		if (A.size() && B.size()) {
			auto g2 = convex_hull(minkowski_sum(A, B));
			vp all;
			for (auto u : A) for (auto v : B) all.push_back(u + v);
			auto bf = brute_hull(all);
			bool ok = true;
			if (g2.size() >= 3 && bf.size() >= 3) {
				for (auto p : g2) if (!pt_in_hull(bf, p)) ok = false;
				for (auto p : bf) if (!pt_in_hull(g2, p)) ok = false;
			}
			if (!ok) oops("mink_large");
		}
		if (fail > 80) break;
	}
	cout << "geo_hull small_fail=" << sf << " mid_fail=" << (sf2 - sf)
		 << " total_fail=" << fail << (fail ? " FAIL\n" : " OK\n");
	return fail ? 1 : 0;
}
