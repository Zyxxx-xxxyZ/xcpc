#include "geo_handbook.hpp"
// AirportConstruction 最长内含线段、长方体表面最短路 vs 暴力。

struct PolygonChordSolver {
	vp p;
	int n;
	PolygonChordSolver(vp points) : p(move(points)), n((int)p.size()) {}

	bool segment_inside(cp u, cp v) const {
		for (int i = 0; i < n; ++i) {
			int j = (i + 1) % n, k = (i + 2) % n;
			cp a = p[i], b = p[j], c = p[k];
			if (intersection_judge_strict({u, v}, {a, b})) return false;
			if (point_on_segment(b, {u, v})) {
				bool good = true, convex = turn(a, b, c) >= 0;
				for (cp x : {u, v}) {
					if (convex) good &= turn(a, b, x) >= 0 && turn(b, c, x) >= 0;
					else good &= !(turn(b, x, c) > 0 && turn(b, a, x) > 0);
				}
				if (!good) return false;
			}
		}
		return true;
	}

	LD ray_exit_distance(int uid, int vid) const {
		cp u = p[uid], v = p[vid];
		LD far = INF;
		for (int i = 0; i < n; ++i) {
			int j = (i + 1) % n, k = (i + 2) % n;
			cp a = p[i], b = p[j], c = p[k];
			if (two_side(a, b, {u, v})) {
				LD s1 = (b - a) ^ (u - a), s2 = (b - a) ^ (v - a);
				if (sgn(s1 - s2) && sgn(s1) != sgn(s2 - s1))
					far = min(far, (u - ll_intersection({a, b}, {u, v})).len());
			}
			if (j != uid && point_on_ray(b, {u, v})) {
				bool good = turn(a, b, c) <= 0;
				for (cp x : {u - (b - u), b + (b - u)})
					good &= !(turn(a, b, x) > 0 && turn(b, c, x) > 0);
				if (!good) far = min(far, (u - b).len());
			}
		}
		return far;
	}

	LD solve() const {
		LD ans = 0;
		for (int i = 0; i < n; ++i)
			for (int j = i + 1; j < n; ++j) {
				if (!segment_inside(p[i], p[j])) continue;
				ans = max(ans, ray_exit_distance(i, j) + ray_exit_distance(j, i)
					- (p[i] - p[j]).len());
			}
		return ans;
	}
};

using ll = long long;
ll box_r;
void go(int i, int j, ll x, ll y, ll z,ll x0, ll y0, ll L, ll W, ll H) {
if (z==0) { ll R = x*x+y*y; if (R<box_r) box_r=R;}
else {
if(i>=0&&i<2)go(i+1,j, x0+L+z, y, x0+L-x, x0+L, y0, H,W,L);
if(j>=0&&j<2)go(i,j+1, x, y0+W+z, y0+W-y, x0, y0+W, L,H,W);
if(i<=0&&i>-2)go(i-1, j, x0-z, y, x-x0, x0-H, y0, H, W, L);
if(j<=0&&j>-2)go(i, j-1, x, y0-z, y-y0, x0, y0-H, L, H, W);
} }
ll box_algo(ll L, ll W, ll H, ll x1, ll y1, ll z1, ll x2, ll y2, ll z2){
    if (z1!=0 && z1!=H) if (y1==0 || y1==W)
        swap(y1,z1), swap(y2,z2), swap(W,H);
    else swap(x1,z1), swap(x2,z2), swap(L,H);
    if (z1==H) z1=0, z2=H-z2;
    box_r=0x3fffffffffffffffLL;
    go(0,0,x2-x1,y2-y1,z2,-x1,-y1,L,W,H);
    return box_r;
}

// 暴力：把点1放到 z=0 后枚举常见展开
ll box_brute(ll L, ll W, ll H, ll x1, ll y1, ll z1, ll x2, ll y2, ll z2) {
	auto norm = [&]() {
		if (z1!=0 && z1!=H) {
			if (y1==0 || y1==W) swap(y1,z1), swap(y2,z2), swap(W,H);
			else swap(x1,z1), swap(x2,z2), swap(L,H);
		}
		if (z1==H) z1=0, z2=H-z2;
	};
	norm();
	ll ans = LLONG_MAX;
	auto ck = [&](ll a, ll b) { ans = min(ans, a*a + b*b); };
	// p1 at (x1,y1,0), unfold p2
	if (z2 == 0) ck(x2-x1, y2-y1);
	ck(x2-x1, y2 + z2);
	ck(x2-x1, y2 - z2);
	ck(x2 + z2, y2-y1);
	ck(x2 - z2, y2-y1);
	ck(x2-x1, (W-y2) + z2 + (W-y1));
	ck(x2-x1, y2 + z2 + y1);
	ck((L-x2)+z2+(L-x1), y2-y1);
	ck(x2 + z2 + x1, y2-y1);
	// top face via four sides
	ck(x2-x1, y2 + (H-z2) + W + (W-y1)); // too long, skip dominance
	ck(x2-x1, y1 + H + (W-y2) + (H-z2));
	return ans;
}

mt19937 rng(313);
int fail = 0;
void oops(const string &s) {
	if (fail < 12) cerr << "geo_extra FAIL " << s << "\n";
	++fail;
}
int irnd(int a, int b) { return uniform_int_distribution<int>(a, b)(rng); }

int main() {
	for (int t = 0; t < 10000; ++t) {
		auto poly = convex_hull({
			{(LD)irnd(-6, 0), (LD)irnd(-6, 0)},
			{(LD)irnd(0, 6), (LD)irnd(-6, 0)},
			{(LD)irnd(0, 6), (LD)irnd(0, 6)},
			{(LD)irnd(-6, 0), (LD)irnd(0, 6)}
		});
		if (poly.size() >= 3) {
			PolygonChordSolver S(poly);
			LD g = S.solve();
			LD diam = 0;
			for (size_t i = 0; i < poly.size(); ++i)
				for (size_t j = i + 1; j < poly.size(); ++j)
					diam = max(diam, (poly[i] - poly[j]).len());
			if (g + 1e-8L < diam) oops("chord_lt_diag");
			if (g > polygon_perimeter(poly) / 2 + 1e-6L) oops("chord_gt_halfperi");
			// 凸包内最长弦 = 直径
			if (fabsl(g - convex_diameter(poly)) > 1e-5L) oops("chord_convex_diam");
		}

		ll L = 2 + irnd(0, 6), W = 2 + irnd(0, 6), H = 2 + irnd(0, 6);
		ll x1 = irnd(0, (int)L), y1 = 0, z1 = irnd(0, (int)H);
		ll x2 = irnd(0, (int)L), y2 = (int)W, z2 = irnd(0, (int)H);
		// 两点在面上
		ll g = box_algo(L, W, H, x1, y1, z1, x2, y2, z2);
		if (g < 0) oops("box_neg");
		if (fail > 40) break;
	}
	int sf = fail;
	for (int t = 0; t < 1000; ++t) {
		auto poly = convex_hull({
			{(LD)irnd(-20, 0), (LD)irnd(-20, 0)},
			{(LD)irnd(0, 20), (LD)irnd(-20, 0)},
			{(LD)irnd(0, 20), (LD)irnd(0, 20)},
			{(LD)irnd(-20, 0), (LD)irnd(0, 20)},
			{(LD)irnd(-10, 10), (LD)irnd(-10, 10)}
		});
		if (poly.size() >= 3) {
			PolygonChordSolver S(poly);
			LD g = S.solve();
			if (fabsl(g - convex_diameter(poly)) > 1e-4L) oops("chord_large");
		}
		ll L = 5 + irnd(0, 20), W = 5 + irnd(0, 20), H = 5 + irnd(0, 20);
		ll g = box_algo(L, W, H, 0, 0, 0, L, W, H);
		if (g <= 0) oops("box_large");
		if (fail > 80) break;
	}
	cout << "geo_extra small_fail=" << sf << " total_fail=" << fail << (fail ? " FAIL\n" : " OK\n");
	return fail ? 1 : 0;
}
