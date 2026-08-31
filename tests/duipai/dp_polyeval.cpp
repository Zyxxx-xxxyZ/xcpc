#include <bits/stdc++.h>
using namespace std;
using LL = long long;
using ULL = unsigned long long;
using poly = vector<int>;
const int p = 998244353;
const int MAXN = 2048;
const int N = 2048;
int qpow(int a, int k){ int r=1; for(;k;k>>=1,a=(LL)a*a%p) if(k&1) r=(LL)r*a%p; return r; }
vector<int> omega[25];
void ntt_init(int n) {
	for (int k = 2, d = 0; k <= n; k *= 2, d++) {
		omega[d].resize(k + 1);
		int wn = qpow(3, (p - 1) / k), tmp = 1;
		for (int i = 0; i <= k; i++) { omega[d][i] = tmp; tmp = (LL)tmp * wn % p; } } }
void ntt(int *c, int n, int tp) {
	static ULL a[N];
	for (int i = 0; i < n; i++) a[i] = c[i];
	for (int i = 1, j = 0; i < n - 1; i++) {
		int k = n; do j ^= (k >>= 1); while (j < k);
		if (i < j) swap(a[i], a[j]); }
	for (int k = 1, d = 0; k < n; k *= 2, d++) {
		for (int i = 0; i < n; i += k * 2)
			for (int j = 0; j < k; j++) {
				int w = omega[d][tp > 0 ? j : k * 2 - j];
				ULL u = a[i + j], v = w * a[i + j + k] % p;
				a[i + j] = u + v; a[i + j + k] = u - v + p; } }
	if (tp>0) {for (int i = 0; i < n; i++) c[i] = a[i] % p;}
	else { int invn = qpow(n, p - 2); for (int i = 0; i < n; i++) c[i] = (int)(a[i]%p * invn % p);}}
poly poly_calc(const poly& u, const poly& v, function<int(int, int)> op) {
	static int a[MAXN], b[MAXN], c[MAXN];
	int n = (int)u.size();
	int NN = 1; while (NN < n) NN *= 2;
	memcpy(a, u.data(), sizeof(int) * n); fill(a + n, a + NN * 2, 0);
	memcpy(b, v.data(), sizeof(int) * n); fill(b + n, b + NN * 2, 0);
	ntt(a, NN * 2, 1); ntt(b, NN * 2, 1);
	for (int i = 0; i < NN * 2; i++) c[i] = op(a[i], b[i]);
	ntt(c, NN * 2, -1); return poly(c, c + n * 2); }
poly poly_inv(const poly& a) {
	int n = (int)a.size(), NN = 1; while (NN < n) NN *= 2;
	poly aa = a; aa.resize(NN);
	poly c{qpow(aa[0], p - 2)};
	for (int k = 2; k <= NN; k *= 2) {
		c.resize(k); poly b(aa.begin(), aa.begin() + k);
		c = poly_calc(b, c, [](int bi, int ci) {
			return ((2 - (LL)bi * ci) % p + p) * ci % p; });
		memset(c.data() + k, 0, sizeof(int) * k); }
	c.resize(n); return c; }
poly poly_auto_mul(poly a, poly b) {
	int res_len = (int)a.size() + (int)b.size() - 1;
	int ntt_n = 1; while (ntt_n < res_len) ntt_n *= 2;
	a.resize(ntt_n); b.resize(ntt_n);
	ntt(a.data(), ntt_n, 1); ntt(b.data(), ntt_n, 1);
	for (int i = 0; i < ntt_n; i++) a[i] = (LL)a[i] * b[i] % p;
	ntt(a.data(), ntt_n, -1); a.resize(res_len); return a; }
poly poly_div(const poly& a, const poly& b) {
	poly bb = b; while (bb.size() > 1 && bb.back() == 0) bb.pop_back();
	int n = (int)a.size(), m = (int)bb.size();
	if (n < m) return {};
	int ntt_n = 1; while (ntt_n < n - m + 1) ntt_n *= 2;
	poly f(ntt_n), g(ntt_n);
	for (int i = 0; i < n - m + 1; i++) f[i] = a[n - i - 1];
	for (int i = 0; i < m && i < n - m + 1; i++) g[i] = bb[m - i - 1];
	auto g_inv = poly_inv(g);
	fill(g_inv.begin() + n - m + 1, g_inv.end(), 0);
	auto c = poly_auto_mul(f, g_inv); c.resize(n - m + 1);
	reverse(c.begin(), c.end()); return c; }
pair<poly, poly> poly_mod(const poly& a, const poly& b) {
	poly bb = b; while (bb.size() > 1 && bb.back() == 0) bb.pop_back();
	int n = (int)a.size(), m = (int)bb.size();
	if (n < m) { poly r = a; r.resize(max(0, m - 1)); return {r, {}}; }
	auto d = poly_div(a, bb); auto c = poly_auto_mul(bb, d);
	poly r(m - 1);
	for (int i = 0; i < m - 1; i++) {
		int ci = i < (int)c.size() ? c[i] : 0;
		int ai = i < (int)a.size() ? a[i] : 0;
		r[i] = (ai - ci + p) % p; }
	return {r, d}; }
struct poly_eval { poly f; vector<int> x;
	vector<poly> gs; vector<int> ans;
	poly_eval(poly f, vector<int> x) : f(f), x(x) {}
	void pretreat(int l, int r, int o) { poly& g = gs[o];
		if (l == r) { g = poly{p - x[l], 1}; return; }
		int mid = (l + r) / 2; pretreat(l, mid, o * 2);
		pretreat(mid + 1, r, o * 2 + 1);
		if (o > 1) g = poly_auto_mul(gs[o * 2], gs[o * 2 + 1]); }
	void solve(int l, int r, int o, const poly& f) {
		if (l == r) { ans[l] = f[0]; return; }
		int mid = (l + r) / 2;
		solve(l, mid, o * 2, poly_mod(f, gs[o * 2]).first);
		solve(mid + 1, r, o * 2 + 1, poly_mod(f, gs[o * 2 + 1]).first); }
	vector<int> operator() () {
		int n = (int)f.size(), m = (int)x.size(), orig_m = m;
		if (m <= n) x.resize(m = n + 1);
		else if (n < m - 1) f.resize(n = m - 1);
		int bit_ceil = 1; while (bit_ceil < m) bit_ceil *= 2;
		ntt_init(bit_ceil * 2);
		gs.resize(2 * bit_ceil + 1); pretreat(0, m - 1, 1);
		ans.resize(m); solve(0, m - 1, 1, f);
		ans.resize(orig_m); return ans;} };
int horner(const poly& f, int x){
    LL r=0; for(int i=(int)f.size()-1;i>=0;i--) r=(r*x+f[i])%p;
    return (int)r;
}
poly poly_power_mod(LL k, const poly& m) {
	poly ans{1}, a{0, 1}; while (k) { if (k & 1)
			ans = poly_mod(poly_auto_mul(ans, a), m).first;
		a = poly_mod(poly_auto_mul(a, a), m).first; k /= 2; }
	return ans; }
struct linear_recurrence { poly f;
	linear_recurrence(const poly& c, LL n) {
		int m = (int)c.size() - 1;
		int ntt_n = 1; while (ntt_n < m * 2) ntt_n *= 2;
		ntt_init(ntt_n);
		poly t(m + 1); t[m] = 1;
		for (int i = 0; i < m; i++)t[i] = (p - c[m - i]) % p;
		f = poly_power_mod(n, t); f.resize(m, 0); }
	int operator()(const vector<int>& a) {
		int ans = 0;
		for (int i = 0; i < (int)a.size(); i++)
			ans = (ans + (LL)f[i] * a[i]) % p;
		return ans; } };
mt19937 rng(123);
int fail=0;
int main(){
    ntt_init(1024);
    for(int t=0;t<10000;t++){
        int n=1+t%4, m=1+t%5;
        poly f(n); vector<int> xs(m);
        for(int i=0;i<n;i++) f[i]=rng()%p;
        for(int i=0;i<m;i++) xs[i]=rng()%p;
        auto g=poly_eval(f,xs)();
        if((int)g.size()!=m) fail++;
        for(int i=0;i<m;i++) if(g[i]!=horner(f,xs[i])) { fail++; break; }
    }
    int sf=fail;
    for(int t=0;t<1000;t++){
        int n=4+t%6, m=2+t%8;
        poly f(n); vector<int> xs(m);
        for(int i=0;i<n;i++) f[i]=rng()%p;
        for(int i=0;i<m;i++) xs[i]=rng()%p;
        auto g=poly_eval(f,xs)();
        for(int i=0;i<m;i++) if(g[i]!=horner(f,xs[i])) { fail++; break; }
        // linear rec
        int mm=2+t%3; poly c(mm+1); c[0]=0;
        for(int i=1;i<=mm;i++) c[i]=rng()%p;
        vector<int> a(mm); for(int i=0;i<mm;i++) a[i]=rng()%p;
        int N=mm+t%8;
        linear_recurrence lr(c,N);
        vector<int> seq=a;
        for(int i=mm;i<=N;i++){
            LL s=0; for(int j=1;j<=mm;j++) s+=(LL)c[j]*seq[i-j]%p;
            seq.push_back(s%p);
        }
        if(lr(a)!=seq[N]) fail++;
    }
    cout<<"dp_polyeval small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}