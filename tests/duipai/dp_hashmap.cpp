#include <bits/stdc++.h>
#include <cassert>
using namespace std;
using ULL = unsigned long long;
using T = long long;

template <class T,int P>
struct hashmap {
ULL id[P]; T val[P];
int R[P + 1];
hashmap() {memset(id, -1, sizeof id); memset(val, 0, sizeof val); R[0] = 0;}
T get(const ULL &x) const {
	for (int i = int(x % P), j = 1, steps = 0; ~id[i]; i = (i + j) % P, j = (j + 2) % P) {
		if (id[i] == x) return val[i];
		if (++steps >= P) break; }
	return 0; }
T& operator [] (const ULL &x) {
	for (int i = int(x % P), j = 1, steps = 0; ; i = (i + j) % P, j = (j + 2) % P) {
		if (id[i] == x) return val[i];
		else if (id[i] == -1llu) {
			id[i] = x; val[i] = T{};
			R[++R[0]] = i;
			return val[i]; }
		if (++steps >= P) { assert(false); static T dummy{}; return dummy; } } }
void clear() {
for (int &x = R[0]; x; id[R[x]] = -1, val[R[x]] = 0, --x); }
void fullclear() {memset(id, -1, sizeof id); R[0] = 0; } };

mt19937_64 rng(2);
int fail = 0;
void oops(string s) { if (fail < 8) cerr << "FAIL " << s << "\n"; fail++; }

template<int P>
void run_ops(int nops, ULL keymod) {
    hashmap<T, P> h;
    map<ULL, T> gold;
    for (int i = 0; i < nops; i++) {
        int op = rng() % 5;
        ULL k = rng() % keymod;
        if (k == (ULL)-1) k = 0;
        if (op == 0 || op == 1 || gold.size() < 2) { // assign
            if ((int)gold.size() >= P - 1 && !gold.count(k)) continue; // don't fill table
            T v = (T)(rng() % 1000000007ull) - 500000000;
            h[k] = v;
            gold[k] = v;
        } else if (op == 2) { // get
            T a = h.get(k), b = gold.count(k) ? gold[k] : 0;
            if (a != b) oops("get mismatch");
        } else if (op == 3) { // operator[] read
            if (!gold.count(k)) {
                if ((int)gold.size() >= P - 1) continue;
                T a = h[k];
                gold[k] = 0;
                if (a != 0) oops("[] insert default");
            } else {
                if (h[k] != gold[k]) oops("[] read");
            }
        } else { // clear occasionally
            if (rng() % 30 == 0) { h.clear(); gold.clear(); }
        }
    }
    for (auto [k, v] : gold) if (h.get(k) != v) oops("final get");
}

int main() {
    for (int t = 0; t < 10000; t++) run_ops<97>(20 + t % 40, 50);
    int small_fail = fail;
    for (int t = 0; t < 1000; t++) run_ops<10007>(200 + t % 400, 5000);
    cout << "dp_hashmap small_fail=" << small_fail << " total_fail=" << fail << (fail ? " FAIL\n" : " OK\n");
    return fail ? 1 : 0;
}
