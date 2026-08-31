#include <bits/stdc++.h>
#include <cassert>
using namespace std;
using ll = long long;
using pii = pair<int,int>;
const int MOD = 1000000007;
const int N = 64;
vector<int> e[N];
int qpow(int a, int k){ int r=1; for(;k;k>>=1,a=(ll)a*a%MOD) if(k&1) r=(ll)r*a%MOD; return r; }
int reverse(int x){ return qpow(x, MOD-2); }

int ra[N]; void prepare() {
	for (int i = 0; i < N; ++ i) ra[i] = rand() % (MOD - 1) + 1;}
struct Sub {
	vector<int> s; int d1, d2, H1, H2;
	Sub() {d1 = d2 = 0; s.clear();}
	void add(int d, int v) { s.push_back(v);
		if (d>d1) d2=d1, d1=d; else if (d>d2) d2=d; }
	int hash() { H1 = H2 = 1; for (int i : s) {
			H1 = (ll) H1 * (((ll)ra[d1] + i) % MOD) % MOD;
			H2 = (ll) H2 * (((ll)ra[d2] + i) % MOD) % MOD; } return H1;}
	int mul_inv(int d, int v) { int t = (int)(((ll)ra[d] + v) % MOD); if (t < 0) t += MOD; if (!t) return 0; return reverse(t); }
	pii del(int d, int v) { if (d==d1)
		return {d2+1, (ll)H2*mul_inv(d2, v) % MOD};
		return {d1+1, (ll)H1*mul_inv(d1, v) % MOD};}};
pii U[N]; int A[N]; Sub tree[N];
#define fors(i) for (auto i : e[x]) if (i != p)
void dfsD(int x, int p) { tree[x] = Sub();
	fors(i) { dfsD(i, x);
		tree[x].add(tree[i].d1 + 1, tree[i].H1); }
	tree[x].hash(); }
void dfsU(int x, int p) {
	if (p) tree[x].add(U[x].first, U[x].second);
	A[x] = tree[x].hash();
	fors(i){U[i]=tree[x].del(tree[i].d1+1,tree[i].H1);
		dfsU(i, x); } }

string ahu(int x, int p, const vector<vector<int>>& g) {
    vector<string> ch;
    for (int y: g[x]) if (y!=p) ch.push_back(ahu(y,x,g));
    sort(ch.begin(), ch.end());
    string r = "(";
    for (auto& s: ch) r += s;
    r += ")";
    return r;
}
string unrooted_ahu(int n, const vector<pair<int,int>>& edges) {
    vector<vector<int>> g(n+1);
    vector<int> deg(n+1);
    for (auto [u,v]: edges) { g[u].push_back(v); g[v].push_back(u); deg[u]++; deg[v]++; }
    if (n==1) return ahu(1,0,g);
    queue<int> q; vector<int> live=deg;
    vector<int> layer;
    for (int i=1;i<=n;i++) if (live[i]<=1) q.push(i);
    vector<int> cent;
    int left = n;
    while (left > 2) {
        int sz=q.size();
        left -= sz;
        while (sz--) {
            int u=q.front(); q.pop();
            for (int v: g[u]) if (--live[v]==1) q.push(v);
        }
    }
    while (!q.empty()) { cent.push_back(q.front()); q.pop(); }
    vector<string> cs;
    for (int c: cent) cs.push_back(ahu(c,0,g));
    sort(cs.begin(), cs.end());
    string r;
    for (auto& s: cs) r += s;
    return r;
}

vector<int> hashes_of(int n, const vector<pair<int,int>>& edges) {
    for (int i=1;i<=n;i++) e[i].clear();
    for (auto [u,v]: edges) { e[u].push_back(v); e[v].push_back(u); }
    for (int i=0;i<=n;i++) U[i]={0,0}, A[i]=0;
    dfsD(1,0); dfsU(1,0);
    vector<int> h(A+1, A+n+1);
    sort(h.begin(), h.end());
    return h;
}

mt19937 rng(7);
int fail=0;
void oops(string s){ if(fail<10) cerr<<"FAIL "<<s<<"\n"; fail++; }

vector<pair<int,int>> rand_tree(int n) {
    vector<pair<int,int>> ed;
    for (int i=2;i<=n;i++) ed.push_back({i, 1+(int)(rng()%(i-1))});
    return ed;
}
vector<pair<int,int>> relabel(int n, vector<pair<int,int>> ed) {
    vector<int> p(n+1); iota(p.begin(), p.end(), 0);
    shuffle(p.begin()+1, p.end(), rng);
    for (auto& [u,v]: ed) u=p[u], v=p[v];
    return ed;
}

int main() {
    srand(12345);
    prepare();
    for (int t=0;t<10000;t++) {
        int n = 2 + t%8;
        auto ed = rand_tree(n);
        auto ed2 = relabel(n, ed);
        auto h1 = hashes_of(n, ed);
        auto h2 = hashes_of(n, ed2);
        if (h1 != h2) oops("iso hash mismatch n="+to_string(n));
        if (fail>15) break;
    }
    int small_fail=fail;
    for (int t=0;t<1000;t++) {
        int n = 12 + t%20;
        auto ed = rand_tree(n);
        auto ed2 = relabel(n, ed);
        auto h1 = hashes_of(n, ed);
        auto h2 = hashes_of(n, ed2);
        if (h1 != h2) oops("iso large n="+to_string(n));
        if (fail>25) break;
    }
    // different trees: ahu differs => hashes should usually differ (not a hard fail)
    int false_eq=0;
    for (int t=0;t<500;t++) {
        int n=8;
        auto a=rand_tree(n), b=rand_tree(n);
        if (unrooted_ahu(n,a)==unrooted_ahu(n,b)) continue;
        if (hashes_of(n,a)==hashes_of(n,b)) false_eq++;
    }
    cout << "dp_treeiso small_fail=" << small_fail << " total_fail=" << fail
         << " false_eq_noniso=" << false_eq << (fail ? " FAIL\n" : " OK\n");
    return fail ? 1 : 0;
}
