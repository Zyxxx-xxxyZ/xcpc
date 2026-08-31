#include "geo_handbook.hpp"
// 超三角形 LOTS=1e6 是否覆盖随机点（手册 Delaunay 的 contains）。
LD dirv(P a, P b, P p) { return (b - a) ^ (p - a); }
bool tri_contains(P a, P b, P c, P q) {
	return sgn(dirv(a, b, q)) >= 0 && sgn(dirv(b, c, q)) >= 0 && sgn(dirv(c, a, q)) >= 0;
}

int main() {
	const LD LOTS = 1e6;
	P A{-LOTS, -LOTS}, B{+LOTS, -LOTS}, C{0, +LOTS};
	mt19937 rng(403);
	int out_small = 0, out_large = 0;
	for (int t = 0; t < 10000; ++t) {
		int x = (int)(rng() % 2001) - 1000, y = (int)(rng() % 2001) - 1000;
		if (!tri_contains(A, B, C, {(LD)x, (LD)y})) ++out_small;
	}
	for (int t = 0; t < 1000; ++t) {
		int x = (int)(rng() % 20000001) - 10000000;
		int y = (int)(rng() % 20000001) - 10000000;
		if (!tri_contains(A, B, C, {(LD)x, (LD)y})) ++out_large;
	}
	bool known = !tri_contains(A, B, C, {1e6L, 0}) && !tri_contains(A, B, C, {1e7L, 0});
	cout << "geo_delaunay_lots small_out=" << out_small
		 << " large_out=" << out_large << " lots_boundary_out=" << known
		 << (out_large || !known ? " FAIL\n" : " OK\n");
	return (out_large || !known) ? 1 : 0;
}
