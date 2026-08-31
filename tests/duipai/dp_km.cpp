#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int N = 12, M = 12;
ll e[N][M];
vector<int> KM(int n, int m) {
	if (n > m) m = n;
	vector<ll> l(n + 1), r(m + 1);
	vector<int> p(m + 1), ans(n + 1);
	for (int i = 1; i <= n; ++i) {
		l[i] = e[i][1];
		for (int j = 2; j <= m; ++j) l[i] = max(l[i], e[i][j]); }
	for (int i = 1; i <= n; ++i) {
		vector<ll> d(m + 1, 1e18);
		vector<int> pre(m + 1), done(m + 1);
		int x, v = 0, u = 0;
		for (p[0] = i; x = p[u]; u = v) {
			done[u] = 1;
			ll mn = 1e18; v = 0;
			for (int j = 1; j <= m; ++j) if (!done[j]) {
				auto w = l[x] + r[j] - e[x][j];
				if (w < d[j]) d[j] = w, pre[j] = u;
				if (d[j] < mn) mn = d[j], v = j;
			}
			if (!v) break;
			for (int j = 0; j <= m; ++j) {
				if (done[j]) l[p[j]] -= mn, r[j] += mn;
				else d[j] -= mn;
			}
		}
		if (v) for (int vv; u; u = vv) vv = pre[u], p[u] = p[vv];
	}
	for (int j = 1; j <= m; ++j) if (p[j] && p[j] <= n) ans[p[j]] = j;
	return ans; }

ll brute_weight(int n, int m) {
    vector<int> perm(m);
    iota(perm.begin(), perm.end(), 1);
    ll best = (ll)-4e18;
    do {
        ll s = 0;
        for (int i = 1; i <= n; i++) s += e[i][perm[i-1]];
        best = max(best, s);
        reverse(perm.begin()+n, perm.end());
    } while (next_permutation(perm.begin(), perm.end()));
    return best;
}

mt19937 rng(6);
int fail = 0;
void oops(string s) { if (fail<10) cerr << "FAIL " << s << "\n"; fail++; }

void one(int n, int m, int W) {
    memset(e, 0, sizeof e);
    for (int i=1;i<=n;i++) for (int j=1;j<=m;j++)
        e[i][j] = (int)(rng()%(2*W+1)) - W;
    auto ans = KM(n, m);
    int mm = max(n, m);
    ll got = 0;
    vector<int> used(mm+1);
    for (int i=1;i<=n;i++) {
        int j = ans[i];
        if (j<1 || j>mm || used[j]) { oops("invalid matching"); return; }
        used[j]=1;
        if (j <= m) got += e[i][j];
    }
    ll bf;
    if (n <= m) bf = brute_weight(n, m);
    else {
        // pad dummy columns already 0; brute on n columns
        bf = brute_weight(n, n);
    }
    if (got != bf) oops("weight got="+to_string(got)+" bf="+to_string(bf)+" n="+to_string(n)+" m="+to_string(m));
}

int main() {
    for (int t=0;t<10000;t++) {
        int n = 1 + t%4, m = n + t%3; // n<=m, m<=6
        if (m > 6) m = 6;
        if (n > m) n = m;
        one(n, m, 20);
        if (fail>15) break;
    }
    int small_fail = fail;
    for (int t=0;t<1000;t++) {
        int n = 4 + t%3, m = n + t%3; // up to 8
        if (m > 8) m = 8;
        if (n > m) n = m;
        one(n, m, 1000);
        if (fail>25) break;
    }
    // n>m padded
    int nm = fail;
    for (int t=0;t<200;t++) {
        one(4, 3, 15);
        if (fail>30) break;
    }
    cout << "dp_km small_fail=" << small_fail << " large=" << nm-small_fail
         << " n>m=" << fail-nm << " total_fail=" << fail << (fail ? " FAIL\n" : " OK\n");
    return fail ? 1 : 0;
}
