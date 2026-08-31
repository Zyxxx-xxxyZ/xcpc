#include <bits/stdc++.h>
using namespace std;
using LD = long double;
struct P { LD x, y; };
using cp = const P&;
int sgn(LD x) { return fabsl(x) < 1e-9L ? 0 : (x < 0 ? -1 : 1); }
bool operator<(cp a, cp b) { return a.x == b.x ? a.y < b.y : a.x < b.x; }
bool operator==(cp a, cp b) { return !sgn(a.x - b.x) && !sgn(a.y - b.y); }

bool exact_lt(const P& a, const P& b) { return a.x == b.x ? a.y < b.y : a.x < b.x; }
bool equiv(const P& a, const P& b) { return !(a < b) && !(b < a); }

mt19937_64 rng(1);
LD rnd() {
    uniform_real_distribution<double> d(-1000, 1000);
    return (LD)d(rng);
}

int fail = 0;
void oops(string s) { if (fail < 8) cerr << "FAIL " << s << "\n"; fail++; }

void check_swo(vector<P> v) {
    int n = (int)v.size();
    for (int i = 0; i < n; i++) if (v[i] < v[i]) oops("irreflexive");
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) {
        if (v[i] < v[j] && v[j] < v[i]) oops("asym");
        if (equiv(v[i], v[j]) != (v[i].x == v[j].x && v[i].y == v[j].y)) oops("equiv!=exact");
    }
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) for (int k = 0; k < n; k++) {
        if (v[i] < v[j] && v[j] < v[k] && !(v[i] < v[k])) oops("trans <");
        if (equiv(v[i], v[j]) && equiv(v[j], v[k]) && !equiv(v[i], v[k])) oops("trans ~");
    }
    set<P> s(v.begin(), v.end());
    vector<P> a = v; sort(a.begin(), a.end(), exact_lt);
    a.erase(unique(a.begin(), a.end(), [](const P& u, const P& w){ return u.x==w.x && u.y==w.y; }), a.end());
    if (s.size() != a.size()) oops("set size");
}

int main() {
    // small 10000
    for (int t = 0; t < 10000; t++) {
        int n = 3 + t % 8;
        vector<P> v(n);
        for (int i = 0; i < n; i++) v[i] = {rnd(), rnd()};
        if (t % 20 == 0) { // clustered
            v[0] = {0, 0}; v[1] = {0.75, 0}; if (n > 2) v[2] = {1.5, 0};
        }
        check_swo(v);
        if (fail > 20) break;
    }
    int small_fail = fail;
    // large 1000
    for (int t = 0; t < 1000; t++) {
        int n = 40 + t % 80;
        vector<P> v(n);
        for (int i = 0; i < n; i++) v[i] = {rnd(), rnd()};
        // many exact duplicates
        if (n > 5) v[3] = v[0], v[4] = v[1];
        check_swo(v);
        if (fail > 40) break;
    }
    cout << "dp_point small_fail=" << small_fail << " total_fail=" << fail << (fail ? " FAIL\n" : " OK\n");
    return fail ? 1 : 0;
}
