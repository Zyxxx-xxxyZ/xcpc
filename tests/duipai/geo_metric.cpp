#include "geo_handbook.hpp"
// 最近点对、圆上整点、球面距离 vs 暴力。
mt19937 rng(308);
int fail = 0;
void oops(const string &s) {
	if (fail < 15) cerr << "geo_metric FAIL " << s << "\n";
	++fail;
}
int irnd(int a, int b) { return uniform_int_distribution<int>(a, b)(rng); }

LD brute_closest(const vp &p) {
	LD ans = INF;
	for (size_t i = 0; i < p.size(); ++i)
		for (size_t j = i + 1; j < p.size(); ++j)
			ans = min(ans, (p[i] - p[j]).len());
	return ans;
}

set<LL> brute_lattice_y(LL r) {
	set<LL> s;
	for (LL y = 0; y <= r; ++y) {
		LL t = r * r - y * y;
		LL x = (LL)sqrt((long double)t);
		while (x * x < t) ++x;
		while (x && x * x > t) --x;
		if (x * x == t) s.insert(y);
	}
	return s;
}

int main() {
	for (int t = 0; t < 10000; ++t) {
		int n = 2 + t % 10;
		vp pts;
		set<pair<int, int>> st;
		for (int i = 0; i < n * 3 && (int)pts.size() < n; ++i) {
			int x = irnd(-20, 20), y = irnd(-20, 20);
			if (st.insert({x, y}).second) pts.push_back({(LD)x, (LD)y});
		}
		if (pts.size() >= 2) {
			LD g = closest_pair(pts);
			LD b = brute_closest(pts);
			if (fabsl(g - b) > 1e-8L) oops("closest");
		}

		LL r = 1 + t % 80;
		auto gy = lattice_circle_y(r);
		set<LL> gs(gy.begin(), gy.end());
		auto by = brute_lattice_y(r);
		if (gs != by) {
			if (fail < 10) {
				cerr << "lattice r=" << r << " got";
				for (auto x : gs) cerr << " " << x;
				cerr << " bf";
				for (auto x : by) cerr << " " << x;
				cerr << "\n";
			}
			oops("lattice");
		}

		LD lon1 = irnd(-180, 180) * pi / 180;
		LD lat1 = irnd(-89, 89) * pi / 180;
		LD lon2 = irnd(-180, 180) * pi / 180;
		LD lat2 = irnd(-89, 89) * pi / 180;
		LD R = 10;
		LD g = sphereDis(lon1, lat1, lon2, lat2, R);
		auto to3 = [&](LD lon, LD lat) {
			return array<LD, 3>{R * cosl(lat) * cosl(lon), R * cosl(lat) * sinl(lon), R * sinl(lat)};
		};
		auto A = to3(lon1, lat1), B = to3(lon2, lat2);
		LD dot = (A[0] * B[0] + A[1] * B[1] + A[2] * B[2]) / (R * R);
		LD bf = R * acosl(clamp(dot, -1.0L, 1.0L));
		if (fabsl(g - bf) > 1e-8L) oops("sphere");
		if (fail > 50) break;
	}
	int sf = fail;
	for (int t = 0; t < 1000; ++t) {
		int n = 20 + t % 20;
		vp pts;
		set<pair<int, int>> st;
		for (int i = 0; i < n * 3 && (int)pts.size() < n; ++i) {
			int x = irnd(-200, 200), y = irnd(-200, 200);
			if (st.insert({x, y}).second) pts.push_back({(LD)x, (LD)y});
		}
		if (pts.size() >= 2) {
			if (fabsl(closest_pair(pts) - brute_closest(pts)) > 1e-7L) oops("closest_large");
		}
		LL r = 81 + t * 13;
		if (r > 4000) r = 81 + t;
		auto gy = lattice_circle_y(r);
		set<LL> gs(gy.begin(), gy.end());
		auto by = brute_lattice_y(r);
		if (gs != by) oops("lattice_large");
		if (fail > 80) break;
	}
	cout << "geo_metric small_fail=" << sf << " total_fail=" << fail << (fail ? " FAIL\n" : " OK\n");
	return fail ? 1 : 0;
}
