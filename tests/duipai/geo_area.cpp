#include "geo_handbook.hpp"
// 圆并、多边形与圆交 vs 独立公式 / 网格。
mt19937 rng(310);
int fail = 0;
void oops(const string &s) {
	if (fail < 12) cerr << "geo_area FAIL " << s << "\n";
	++fail;
}
int irnd(int a, int b) { return uniform_int_distribution<int>(a, b)(rng); }

const int MAXN = 32;
int circle_count;
C circles[MAXN];
LD coverage_area[MAXN];

struct Event {
	LD ang;
	int delta;
	bool operator<(const Event &o) const { return ang < o.ang; }
};

bool same_circle(cc a, cc b) {
	return a.c == b.c && !sgn(a.r - b.r);
}
bool contains(cc a, cc b) {
	return sgn(a.r - b.r - (a.c - b.c).len()) >= 0;
}

void add_interval(LD l, LD r, vector<Event> &e, int &base) {
	const LD tau = 2 * pi;
	while (l < 0) l += tau, r += tau;
	while (l >= tau) l -= tau, r -= tau;
	if (r < tau) e.push_back({l, 1}), e.push_back({r, -1});
	else {
		++base;
		e.push_back({r - tau, -1});
		e.push_back({l, 1});
	}
}

void circle_union() {
	fill(coverage_area, coverage_area + circle_count + 2, 0);
	vector<LD> delta(circle_count + 2);
	for (int i = 0; i < circle_count; ++i) {
		bool duplicate = false;
		for (int j = 0; j < i; ++j)
			if (same_circle(circles[i], circles[j])) duplicate = true;
		if (duplicate) continue;

		int multiplicity = 0;
		for (int j = 0; j < circle_count; ++j)
			multiplicity += same_circle(circles[i], circles[j]);
		int cnt = multiplicity;
		vector<Event> e;
		for (int j = 0; j < circle_count; ++j) if (i != j) {
			if (same_circle(circles[i], circles[j])) continue;
			if (contains(circles[j], circles[i])) { ++cnt; continue; }
			if (contains(circles[i], circles[j])) continue;
			LD d = (circles[j].c - circles[i].c).len();
			if (sgn(d - circles[i].r - circles[j].r) >= 0) continue;
			LD a = atan2l(circles[j].c.y - circles[i].c.y,
				circles[j].c.x - circles[i].c.x);
			LD b = acosl(clamp((circles[i].r * circles[i].r + d * d
				- circles[j].r * circles[j].r) / (2 * circles[i].r * d),
				-1.0L, 1.0L));
			add_interval(a - b, a + b, e, cnt);
		}
		e.push_back({0, 0});
		e.push_back({2 * pi, 0});
		sort(e.begin(), e.end());
		for (int j = 0; j + 1 < (int)e.size(); ++j) {
			cnt += e[j].delta;
			LD l = e[j].ang, r = e[j + 1].ang, d = r - l;
			P p = circles[i].c + P(cosl(l), sinl(l)) * circles[i].r;
			P q = circles[i].c + P(cosl(r), sinl(r)) * circles[i].r;
			LD area = (p ^ q) / 2
				+ circles[i].r * circles[i].r * (d - sinl(d)) / 2;
			delta[cnt - multiplicity + 1] += area;
			delta[cnt + 1] -= area;
		}
	}
	for (int k = 1; k <= circle_count; ++k)
		coverage_area[k] = coverage_area[k - 1] + delta[k];
	for (int k = 1; k < circle_count; ++k)
		coverage_area[k] -= coverage_area[k + 1];
}

LD union_area() {
	LD s = 0;
	for (int k = 1; k <= circle_count; ++k) s += coverage_area[k];
	return s;
}

LD brute_two_union(C a, C b) {
	return pi * a.r * a.r + pi * b.r * b.r - cc_intersection_area(a, b);
}

LD grid_union(const vector<C> &cs, int G = 80) {
	LD minx = INF, maxx = -INF, miny = INF, maxy = -INF;
	for (auto c : cs) {
		minx = min(minx, c.c.x - c.r); maxx = max(maxx, c.c.x + c.r);
		miny = min(miny, c.c.y - c.r); maxy = max(maxy, c.c.y + c.r);
	}
	if (maxx - minx < eps || maxy - miny < eps) return 0;
	int hit = 0, tot = 0;
	for (int i = 0; i < G; ++i) for (int j = 0; j < G; ++j) {
		LD x = minx + (i + 0.5L) / G * (maxx - minx);
		LD y = miny + (j + 0.5L) / G * (maxy - miny);
		++tot;
		for (auto c : cs) if ((P{x, y} - c.c).len() <= c.r + 1e-15L) { ++hit; break; }
	}
	return (LD)hit / tot * (maxx - minx) * (maxy - miny);
}

int main() {
	for (int t = 0; t < 10000; ++t) {
		C a{{(LD)irnd(-4, 4), (LD)irnd(-4, 4)}, (LD)(1 + irnd(0, 4))};
		C b{{(LD)irnd(-4, 4), (LD)irnd(-4, 4)}, (LD)(1 + irnd(0, 4))};
		if (a.r < eps || b.r < eps) continue;
		circle_count = 2;
		circles[0] = a; circles[1] = b;
		circle_union();
		LD g = union_area();
		LD bf = brute_two_union(a, b);
		if (fabsl(g - bf) > 1e-4L) oops("union2");

		vp poly = convex_hull({
			{(LD)irnd(-5, 0), (LD)irnd(-5, 0)},
			{(LD)irnd(0, 5), (LD)irnd(-5, 0)},
			{(LD)irnd(0, 5), (LD)irnd(0, 5)},
			{(LD)irnd(-5, 0), (LD)irnd(0, 5)}
		});
		if (poly.size() >= 3) {
			C c{{(LD)irnd(-3, 3), (LD)irnd(-3, 3)}, (LD)(1 + irnd(0, 4))};
			LD ga = polygon_circle_intersection_area(poly, c);
			if (ga < -1e-8L) oops("pc_neg");
			if (ga > polygon_area(poly) + 1e-4L) oops("pc_gt_poly");
			if (ga > pi * c.r * c.r + 1e-4L) oops("pc_gt_circle");
		}
		if (fail > 40) break;
	}
	int sf = fail;
	for (int t = 0; t < 1000; ++t) {
		int n = 2 + t % 3;
		circle_count = n;
		vector<C> cs;
		for (int i = 0; i < n; ++i) {
			circles[i] = {{(LD)irnd(-6, 6), (LD)irnd(-6, 6)}, (LD)(1 + irnd(0, 5))};
			cs.push_back(circles[i]);
		}
		circle_union();
		LD g = union_area();
		LD gd = grid_union(cs, 70);
		if (fabsl(g - gd) > 2.5L) oops("union_grid");
		if (fail > 80) break;
	}
	cout << "geo_area small_fail=" << sf << " total_fail=" << fail << (fail ? " FAIL\n" : " OK\n");
	return fail ? 1 : 0;
}
