#include "geo_handbook.hpp"
// 射线相交：手册 ray_intersection_judge vs 整数端点的精确有理参数。
mt19937 rng(401);
int irnd(int a, int b) { return uniform_int_distribution<int>(a, b)(rng); }

int main() {
	int fn = 0, fp = 0, tot = 0;
	auto once = [&](int bound) {
		int ax = irnd(-bound, bound), ay = irnd(-bound, bound);
		int bx = irnd(-bound, bound), by = irnd(-bound, bound);
		int cx = irnd(-bound, bound), cy = irnd(-bound, bound);
		int dx = irnd(-bound, bound), dy = irnd(-bound, bound);
		if (ax == bx && ay == by) return;
		if (cx == dx && cy == dy) return;
		long long cr = 1LL * (bx - ax) * (dy - cy) - 1LL * (by - ay) * (dx - cx);
		if (cr == 0) return;
		long long tnum = 1LL * (cx - ax) * (dy - cy) - 1LL * (cy - ay) * (dx - cx);
		long long snum = 1LL * (cx - ax) * (by - ay) - 1LL * (cy - ay) * (bx - ax);
		bool exact = (tnum == 0 || ((tnum > 0) == (cr > 0)))
			&& (snum == 0 || ((snum > 0) == (cr > 0)));
		P pa{(LD)ax, (LD)ay}, pb{(LD)bx, (LD)by};
		P pc{(LD)cx, (LD)cy}, pd{(LD)dx, (LD)dy};
		bool g = ray_intersection_judge({pa, pb}, {pc, pd});
		++tot;
		if (g != exact) {
			if (!g && exact) {
				if (fn < 6)
					cerr << "FN " << ax << "," << ay << "->" << bx << "," << by
						 << " | " << cx << "," << cy << "->" << dx << "," << dy
						 << " t=" << tnum << "/" << cr << " s=" << snum << "/" << cr << "\n";
				++fn;
			} else ++fp;
		}
	};
	for (int t = 0; t < 10000; ++t) once(30);
	int sfn = fn, sfp = fp, stot = tot;
	for (int t = 0; t < 1000; ++t) once(200);
	cout << "geo_ray small tot=" << stot << " FN=" << sfn << " FP=" << sfp
		 << " all tot=" << tot << " FN=" << fn << " FP=" << fp
		 << (fn || fp ? " FAIL\n" : " OK\n");
	return (fn || fp) ? 1 : 0;
}
