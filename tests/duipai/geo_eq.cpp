#include "geo_handbook.hpp"
// operator< 精确 vs operator== eps：对拍 10000 组近重点。
int main() {
	int both = 0;
	mt19937 rng(402);
	uniform_real_distribution<double> d(-1, 1);
	for (int t = 0; t < 10000; ++t) {
		P a{d(rng), d(rng)};
		P b{a.x + (t % 2 ? 1 : -1) * 1e-13L, a.y};
		if ((a < b || b < a) && a == b) ++both;
	}
	for (int t = 0; t < 1000; ++t) {
		P a{(LD)t, 0}, b{a.x + 1e-13L, 0};
		if ((a < b || b < a) && a == b) ++both;
	}
	cout << "geo_eq both_lt_and_eq=" << both << (both ? " FAIL\n" : " OK\n");
	return both ? 1 : 0;
}
