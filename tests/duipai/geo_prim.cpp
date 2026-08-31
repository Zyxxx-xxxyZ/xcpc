#include "geo_handbook.hpp"
// 点、线、射线、线段、直线交：手册 8.2 / 8.3 对暴力。
mt19937 rng(301);
int fail = 0;
void oops(const string &s) {
	if (fail < 12) cerr << "geo_prim FAIL " << s << "\n";
	++fail;
}

int irnd(int a, int b) { return uniform_int_distribution<int>(a, b)(rng); }
P ip() { return {(LD)irnd(-12, 12), (LD)irnd(-12, 12)}; }

bool brute_on_seg(P a, P s, P t) {
	long long ax = llround((double)a.x), ay = llround((double)a.y);
	long long sx = llround((double)s.x), sy = llround((double)s.y);
	long long tx = llround((double)t.x), ty = llround((double)t.y);
	long long cr = (tx - sx) * (ay - sy) - (ty - sy) * (ax - sx);
	if (cr != 0) return false;
	long long dot = (ax - sx) * (ax - tx) + (ay - sy) * (ay - ty);
	return dot <= 0;
}

int brute_ori(P a, P b, P c) {
	long long ax = llround((double)a.x), ay = llround((double)a.y);
	long long bx = llround((double)b.x), by = llround((double)b.y);
	long long cx = llround((double)c.x), cy = llround((double)c.y);
	long long cr = (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);
	return cr > 0 ? 1 : (cr < 0 ? -1 : 0);
}

bool brute_seg_inter(P a, P b, P c, P d) {
	if (brute_on_seg(a, c, d) || brute_on_seg(b, c, d) || brute_on_seg(c, a, b) || brute_on_seg(d, a, b))
		return true;
	return brute_ori(a, b, c) * brute_ori(a, b, d) < 0 && brute_ori(c, d, a) * brute_ori(c, d, b) < 0;
}

bool brute_on_ray(P a, P s, P t) {
	if (s.x == t.x && s.y == t.y) return a.x == s.x && a.y == s.y;
	if (brute_ori(s, t, a) != 0) return false;
	long long ax = llround((double)a.x), ay = llround((double)a.y);
	long long sx = llround((double)s.x), sy = llround((double)s.y);
	long long tx = llround((double)t.x), ty = llround((double)t.y);
	return (ax - sx) * (tx - sx) + (ay - sy) * (ty - sy) >= 0;
}

void one_group(int B) {
	P a = {(LD)irnd(-B, B), (LD)irnd(-B, B)};
	P b = {(LD)irnd(-B, B), (LD)irnd(-B, B)};
	P c = {(LD)irnd(-B, B), (LD)irnd(-B, B)};
	P d = {(LD)irnd(-B, B), (LD)irnd(-B, B)};

	P r90 = a.rot90();
	if (r90.x != -a.y || r90.y != a.x) oops("rot90");
	P r270 = a._rot90();
	if (r270.x != a.y || r270.y != -a.x) oops("_rot90");
	if (fabsl(a.len2() - (a.x * a.x + a.y * a.y)) > 1e-18L) oops("len2");
	if (fabsl((a + b).x - a.x - b.x) + fabsl((a + b).y - a.y - b.y) > 0) oops("add");
	if (fabsl((a * b) - (a.x * b.x + a.y * b.y)) > 0) oops("dot");
	if (fabsl((a ^ b) - (a.x * b.y - a.y * b.x)) > 0) oops("cross");
	if (turn(a, b, c) != brute_ori(a, b, c)) oops("turn");

	P u = a - P{};
	int hf = half(u);
	int hf_bf = (u.y > 0 || (u.y == 0 && u.x > 0));
	if (hf != hf_bf) oops("half");

	L s1{a, b}, s2{c, d};
	bool gs = intersection_judge(s1, s2);
	bool bs = brute_seg_inter(a, b, c, d);
	if (gs != bs) oops("seg_inter");

	bool gr = point_on_ray(c, s1);
	bool br = brute_on_ray(c, a, b);
	if (gr != br) oops("on_ray");

	bool gseg = point_on_segment(c, s1);
	bool bseg = brute_on_seg(c, a, b);
	if (gseg != bseg) oops("on_seg");

	if (a != b) {
		LD dist = point_to_line(c, s1);
		LD dist_bf = fabsl((b - a) ^ (c - a)) / (b - a).len();
		if (fabsl(dist - dist_bf) > 1e-9L) oops("pt_line_dist");
	}

	LD st = segment_to_segment(s1, s2);
	if (brute_seg_inter(a, b, c, d)) {
		if (sgn(st) != 0) oops("segdist_zero");
	} else {
		LD bf = min({point_to_segment(a, s2), point_to_segment(b, s2),
			point_to_segment(c, s1), point_to_segment(d, s1)});
		if (fabsl(st - bf) > 1e-8L) oops("segdist");
	}
	// 重建交点再 point_on_line / ray_intersection_judge：见 geo_ray.cpp（G2）
}

int main() {
	for (int t = 0; t < 10000; ++t) {
		one_group(8);
		if (fail > 40) break;
	}
	int sf = fail;
	for (int t = 0; t < 1000; ++t) {
		one_group(80);
		if (fail > 80) break;
	}
	cout << "geo_prim small_fail=" << sf << " total_fail=" << fail << (fail ? " FAIL\n" : " OK\n");
	return fail ? 1 : 0;
}
