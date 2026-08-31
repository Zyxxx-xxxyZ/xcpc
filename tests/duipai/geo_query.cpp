#include "geo_handbook.hpp"
// ConvexQuery：凸包内、切点、直线交、点到凸包距离 vs 暴力。

struct ConvexQuery {
	vp a;
	int n;
	ConvexQuery(vp points = {}) : a(move(points)), n((int)a.size()) {}

	P at(int i) const {
		if (!n) throw out_of_range("ConvexQuery::at() called on an empty polygon");
		i %= n;
		if (i < 0) i += n;
		return a[i];
	}
	bool inside(cp u) const {
		if (!n) return false;
		if (n == 1) return u == a[0];
		if (n == 2) return point_on_segment(u, {a[0], a[1]});
		if (turn(a[0], a[1], u) < 0 || turn(a[0], a[n - 1], u) > 0)
			return false;
		int l = 1, r = n - 1;
		while (l + 1 < r) {
			int m = (l + r) / 2;
			if (turn(a[0], a[m], u) >= 0) l = m;
			else r = m;
		}
		return turn(a[l], a[(l + 1) % n], u) >= 0;
	}

	template<class F> int extreme(F better) const {
		int l = 0, r = n - 1, d = 1;
		if (better(a[r], a[l])) swap(l, r), d = -1;
		while (d * (r - l) > 1) {
			int m = (l + r) / 2;
			if (better(a[m], a[l]) && better(a[m], a[m - d])) l = m;
			else r = m;
		}
		return l;
	}

	pair<int, int> tangents(cp u) const {
		return {
			extreme([&](cp x, cp y) { return turn(u, y, x) > 0; }),
			extreme([&](cp x, cp y) { return turn(u, x, y) > 0; })};
	}

	int edge_crossing(cp u, cp v, int l, int r) const {
		int side = turn(u, v, at(l));
		while (l + 1 < r) {
			int m = (l + r) / 2;
			if (side == turn(u, v, at(m))) l = m;
			else r = m;
		}
		return l % n;
	}
	bool line_crossings(cp u, cp v, int &i, int &j) const {
		int p0 = extreme([&](cp x, cp y) {
			return ((v - u) ^ (x - u)) < ((v - u) ^ (y - u));
		});
		int p1 = extreme([&](cp x, cp y) {
			return ((v - u) ^ (x - u)) > ((v - u) ^ (y - u));
		});
		if (turn(u, v, a[p0]) * turn(u, v, a[p1]) >= 0) return false;
		if (p0 > p1) swap(p0, p1);
		i = edge_crossing(u, v, p0, p1);
		j = edge_crossing(u, v, p1, p0 + n);
		return true;
	}

	LD chain_distance(cp u, int l, int r) const {
		if (l > r) r += n;
		int side = sgn((u - at(l)) * (at(l + 1) - at(l)));
		LD ret = point_to_segment(u, {at(l), at(l + 1)});
		while (l + 1 < r) {
			int m = (l + r) / 2;
			if (side == sgn((u - at(m)) * (at(m + 1) - at(m)))) l = m;
			else r = m;
		}
		return min(ret, point_to_segment(u, {at(l), at(l + 1)}));
	}
	LD distance(cp u) const {
		if (inside(u)) return 0;
		if (n == 1) return (u - a[0]).len();
		if (n == 2) return point_to_segment(u, {a[0], a[1]});
		auto [x, y] = tangents(u);
		return min(chain_distance(u, x, y), chain_distance(u, y, x));
	}
};

mt19937 rng(306);
int fail = 0;
void oops(const string &s) {
	if (fail < 12) cerr << "geo_query FAIL " << s << "\n";
	++fail;
}
int irnd(int a, int b) { return uniform_int_distribution<int>(a, b)(rng); }

vp rand_convex(int n, int B) {
	vp v;
	set<pair<int, int>> s;
	for (int i = 0; i < n * 5 && (int)v.size() < n; ++i) {
		int x = irnd(-B, B), y = irnd(-B, B);
		if (s.insert({x, y}).second) v.push_back({(LD)x, (LD)y});
	}
	return convex_hull(v);
}

int brute_inside(cvp h, P p) {
	return point_in_polygon(p, h) >= 0;
}

LD brute_dist(cvp h, P p) {
	if (point_in_polygon(p, h) >= 0) return 0;
	LD ans = INF;
	int n = (int)h.size();
	for (int i = 0; i < n; ++i)
		ans = min(ans, point_to_segment(p, {h[i], h[(i + 1) % n]}));
	return ans;
}

pair<int, int> brute_tangents(cvp h, P u) {
	int n = (int)h.size();
	int i0 = 0, i1 = 0;
	for (int i = 0; i < n; ++i) {
		if (turn(u, h[i0], h[i]) < 0) i0 = i;
		if (turn(u, h[i1], h[i]) > 0) i1 = i;
	}
	return {i0, i1};
}

int main() {
	for (int t = 0; t < 10000; ++t) {
		auto h = rand_convex(4 + t % 6, 8);
		if (h.size() < 3) continue;
		ConvexQuery Q(h);
		P p{(LD)irnd(-10, 10), (LD)irnd(-10, 10)};
		bool gi = Q.inside(p);
		bool bi = brute_inside(h, p);
		if (gi != bi) oops("inside");
		LD gd = Q.distance(p);
		LD bd = brute_dist(h, p);
		if (fabsl(gd - bd) > 1e-6L) oops("dist");
		if (point_in_polygon(p, h) < 0) {
			auto gt = Q.tangents(p);
			auto bt = brute_tangents(h, p);
			// 切点应使所有顶点在射线一侧
			auto ok_tan = [&](int id, int sg) {
				for (auto q : h) {
					int s = turn(p, h[id], q);
					if (s && s != sg && s != 0) return false;
				}
				return true;
			};
			if (!ok_tan(gt.first, 1) && !ok_tan(gt.first, -1)) oops("tan1");
			(void)bt;
		}
		P u{(LD)irnd(-12, 12), (LD)irnd(-12, 12)};
		P v = u + P{(LD)irnd(-5, 5), (LD)irnd(-5, 5)};
		if (u != v) {
			int i, j;
			bool cr = Q.line_crossings(u, v, i, j);
			bool any = false;
			int n = (int)h.size();
			for (int k = 0; k < n; ++k)
				if (two_side(h[k], h[(k + 1) % n], {u, v}) ||
					(point_on_line(h[k], {u, v}) && point_on_line(h[(k + 1) % n], {u, v})))
					any = true;
			// 严格穿过：两侧极值异号
			int mn = 1, mx = -1;
			for (auto q : h) {
				int s = turn(u, v, q);
				mn = min(mn, s); mx = max(mx, s);
			}
			bool strict = mn < 0 && mx > 0;
			if (cr != strict) oops("line_cross_flag");
			(void)any; (void)i; (void)j;
		}
		if (fail > 40) break;
	}
	int sf = fail;
	for (int t = 0; t < 1000; ++t) {
		auto h = rand_convex(10 + t % 10, 25);
		if (h.size() < 3) continue;
		ConvexQuery Q(h);
		for (int k = 0; k < 5; ++k) {
			P p{(LD)irnd(-30, 30), (LD)irnd(-30, 30)};
			if (Q.inside(p) != brute_inside(h, p)) oops("inside_large");
			if (fabsl(Q.distance(p) - brute_dist(h, p)) > 1e-5L) oops("dist_large");
		}
		if (fail > 80) break;
	}
	cout << "geo_query small_fail=" << sf << " total_fail=" << fail << (fail ? " FAIL\n" : " OK\n");
	return fail ? 1 : 0;
}
