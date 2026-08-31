#include "geo_handbook.hpp"
// 圆：外接圆、最小圆覆盖、直线/圆交、圆交、切线、反演 vs 暴力。
mt19937 rng(304);
int fail = 0;
void oops(const string &s) {
	if (fail < 15) cerr << "geo_circle FAIL " << s << "\n";
	++fail;
}
int irnd(int a, int b) { return uniform_int_distribution<int>(a, b)(rng); }
P ip(int B) { return {(LD)irnd(-B, B), (LD)irnd(-B, B)}; }

bool covers(const C &c, const vp &pts) {
	for (auto p : pts) if (!in_circle(p, c)) return false;
	return true;
}

C brute_min_circle(vp pts) {
	if (pts.empty()) return {};
	C best{pts[0], INF};
	int n = (int)pts.size();
	auto better = [&](C c) {
		if (!isfinite((double)c.r) || c.r < 0) return;
		if (covers(c, pts) && c.r < best.r) best = c;
	};
	for (int i = 0; i < n; ++i) better(C(pts[i], 0));
	for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j)
		better(C(pts[i], pts[j]));
	for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j)
		for (int k = j + 1; k < n; ++k) {
			if (!turn(pts[i], pts[j], pts[k])) continue;
			better(C(pts[i], pts[j], pts[k]));
		}
	if (best.r > INF / 2) best.r = 0;
	return best;
}

int brute_cc_count(C a, C b) {
	LD d = (a.c - b.c).len();
	if (a.c == b.c) {
		if (sgn(a.r - b.r)) return 0;
		return !sgn(a.r) ? 1 : -1;
	}
	if (sgn(d - a.r - b.r) > 0 || sgn(d - fabsl(a.r - b.r)) < 0) return 0;
	if (!sgn(d - a.r - b.r) || !sgn(d - fabsl(a.r - b.r))) return 1;
	return 2;
}

int main() {
	for (int t = 0; t < 10000; ++t) {
		P a = ip(8), b = ip(8), c = ip(8);
		if (turn(a, b, c)) {
			C cir(a, b, c);
			if (fabsl((cir.c - a).len() - cir.r) > 1e-7L) oops("circum_r_a");
			if (fabsl((cir.c - b).len() - cir.r) > 1e-7L) oops("circum_r_b");
			if (fabsl((cir.c - c).len() - cir.r) > 1e-7L) oops("circum_r_c");
		}

		int n = 2 + t % 6;
		vp pts;
		set<pair<int, int>> st;
		for (int i = 0; i < n * 3 && (int)pts.size() < n; ++i) {
			P p = ip(6);
			if (st.insert({(int)p.x, (int)p.y}).second) pts.push_back(p);
		}
		if (!pts.empty()) {
			C g = min_circle(pts);
			bool nan = !isfinite((double)g.r) || !isfinite((double)g.c.x);
			if (nan) oops("min_circle_nan");
			else {
				if (!covers(g, pts)) oops("min_circle_cover");
				C bf = brute_min_circle(pts);
				if (isfinite((double)bf.r) && g.r > bf.r + 1e-6L) oops("min_circle_not_min");
			}
		}

		C ca{ip(5), (LD)(1 + irnd(0, 5))};
		C cb{ip(5), (LD)(1 + irnd(0, 5))};
		if (cc_intersection_count(ca, cb) != brute_cc_count(ca, cb)) oops("cc_count");
		auto ips = cc_intersection(ca, cb);
		int cnt = cc_intersection_count(ca, cb);
		if (cnt > 0 && (int)ips.size() != (cnt == -1 ? 0 : cnt) && !(ca.c == cb.c && cnt == 1)) {
			if (cnt > 0 && (int)ips.size() != cnt) oops("cc_pts_sz");
		}
		for (auto p : ips) {
			if (fabsl((p - ca.c).len() - ca.r) > 1e-6L) oops("cc_on_a");
			if (fabsl((p - cb.c).len() - cb.r) > 1e-6L) oops("cc_on_b");
		}

		L ln{ip(6), ip(6)};
		if (ln.s != ln.t) {
			auto q = lc_intersection(ln, ca);
			for (auto p : q) {
				if (!point_on_line(p, ln)) oops("lc_on_line");
				if (fabsl((p - ca.c).len() - ca.r) > 1e-6L) oops("lc_on_circle");
			}
		}

		// 点到圆切线：切点应满足半径垂直切线
		P pt = ip(7);
		auto tgs = tangent(pt, ca);
		for (auto p : tgs) {
			if (fabsl((p - ca.c).len() - ca.r) > 1e-6L) oops("tg_on_circle");
			if (sgn((p - ca.c) * (p - pt)) != 0 && fabsl((p - ca.c) * (p - pt)) > 1e-5L)
				oops("tg_perp");
		}

		// 反演：圆外点
		P O = ip(4);
		C A{ip(4) + P{8, 0}, 2};
		if ((A.c - O).len() > A.r + 1) {
			C B = inv_c2c(O, 5, A);
			// |P-O| |P'-O| = R^2 for a point on A mapped to B
			P dir = (A.c - O).unit();
			P far = A.c + dir * A.r;
			P near = A.c - dir * A.r;
			auto invp = [&](P p) {
				return O + (p - O) * (25.0L / (p - O).len2());
			};
			P far2 = invp(far), near2 = invp(near);
			if (fabsl((far2 - B.c).len() - B.r) > 1e-5L) oops("inv_far");
			if (fabsl((near2 - B.c).len() - B.r) > 1e-5L) oops("inv_near");
		}

		if (fail > 50) break;
	}
	int sf = fail;
	for (int t = 0; t < 1000; ++t) {
		vp pts;
		set<pair<int, int>> st;
		int n = 8 + t % 8;
		for (int i = 0; i < n * 3 && (int)pts.size() < n; ++i) {
			P p = ip(20);
			if (st.insert({(int)p.x, (int)p.y}).second) pts.push_back(p);
		}
		C g = min_circle(pts);
		if (!isfinite((double)g.r)) oops("min_circle_nan_large");
		else if (!covers(g, pts)) oops("min_circle_cover_large");
		else {
			C bf = brute_min_circle(pts);
			if (g.r > bf.r + 1e-5L) oops("min_circle_not_min_large");
		}
		if (fail > 80) break;
	}
	cout << "geo_circle small_fail=" << sf << " total_fail=" << fail << (fail ? " FAIL\n" : " OK\n");
	return fail ? 1 : 0;
}
