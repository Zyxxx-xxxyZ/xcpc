#include <bits/stdc++.h>
#include <cassert>
using namespace std;
using LL = long long;
using ULL = unsigned long long;
using poly = vector<int>;
const int p = 998244353;
const int MAXN = 4096;
const int N = 4096;
int qpow(int a, int k){ int r=1; for(;k;k>>=1,a=(LL)a*a%p) if(k&1) r=(LL)r*a%p; return r; }
vector<int> omega[25];
void ntt_init(int n) {
	for (int k = 2, d = 0; k <= n; k *= 2, d++) {
		omega[d].resize(k + 1);
		int wn = qpow(3, (p - 1) / k), tmp = 1;
		for (int i = 0; i <= k; i++) { omega[d][i] = tmp;
			tmp = (LL)tmp * wn % p; } } }
void ntt(int *c, int n, int tp) {
	static ULL a[N];
	for (int i = 0; i < n; i++) a[i] = c[i];
	for (int i = 1, j = 0; i < n - 1; i++) {
		int k = n; do j ^= (k >>= 1); while (j < k);
		if (i < j) swap(a[i], a[j]); }
	for (int k = 1, d = 0; k < n; k *= 2, d++) {
		if (d == 16) for (int i = 0; i < n; i++) a[i] %= p;
		for (int i = 0; i < n; i += k * 2)
			for (int j = 0; j < k; j++) {
				int w = omega[d][tp > 0 ? j : k * 2 - j];
				ULL u = a[i + j], v = w * a[i + j + k] % p;
				a[i + j] = u + v;
				a[i + j + k] = u - v + p; } }
	if (tp>0) {for (int i = 0; i < n; i++) c[i] = a[i] % p;}
	else { int inv = qpow(n, p - 2);
		for (int i = 0; i < n; i++) c[i] = (int)(a[i] % p * inv % p);}}

poly poly_calc(const poly& u, const poly& v,
	function<int(int, int)> op) {
	static int a[MAXN], b[MAXN], c[MAXN];
	int n = (int)u.size(); assert(u.size() == v.size());
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
	for (int i = 0; i < ntt_n; i++)
		a[i] = (LL)a[i] * b[i] % p;
	ntt(a.data(), ntt_n, -1); a.resize(res_len); return a; }
poly poly_div(const poly& a, const poly& b) {
	poly bb = b; while (bb.size() > 1 && bb.back() == 0) bb.pop_back();
	int n = (int)a.size(), m = (int)bb.size();
	if (!m || bb.back() == 0) { assert(false); return {}; }
	if (n < m) return {};
	int ntt_n = 1; while (ntt_n < n - m + 1) ntt_n *= 2;
	poly f(ntt_n), g(ntt_n);
	for (int i = 0; i < n - m + 1; i++) f[i] = a[n - i - 1];
	for (int i = 0; i < m && i < n - m + 1; i++)
		g[i] = bb[m - i - 1];
	auto g_inv = poly_inv(g);
	fill(g_inv.begin() + n - m + 1, g_inv.end(), 0);
	auto c = poly_auto_mul(f, g_inv); c.resize(n - m + 1);
	reverse(c.begin(), c.end()); return c; }
pair<poly, poly> poly_mod(const poly& a, const poly& b) {
	poly bb = b; while (bb.size() > 1 && bb.back() == 0) bb.pop_back();
	if (bb.empty() || bb.back() == 0) { assert(false); return {{}, {}}; }
	int n = (int)a.size(), m = (int)bb.size();
	if (n < m) { poly r = a; r.resize(max(0, m - 1)); return {r, {}}; }
	auto d = poly_div(a, bb); auto c = poly_auto_mul(bb, d);
	poly r(m - 1);
	for (int i = 0; i < m - 1; i++) {
		int ci = i < (int)c.size() ? c[i] : 0;
		int ai = i < (int)a.size() ? a[i] : 0;
		r[i] = (ai - ci + p) % p; }
	return {r, d}; }

poly naive_mul(poly a, poly b) {
    if (a.empty() || b.empty()) return {};
    poly c(a.size()+b.size()-1);
    for (int i=0;i<(int)a.size();i++) for (int j=0;j<(int)b.size();j++)
        c[i+j] = (c[i+j] + (LL)a[i]*b[j]) % p;
    return c;
}
poly trim(poly a) {
    while (a.size()>1 && a.back()==0) a.pop_back();
    while (a.size()>0 && a.back()==0) a.pop_back();
    return a;
}
int deg(const poly& a) {
    int n=(int)a.size()-1; while(n>=0 && a[n]==0) n--; return n;
}

pair<poly,poly> naive_div(poly a, poly b) {
    b = trim(b);
    int m = (int)b.size();
    int ib = qpow(b.back(), p-2);
    poly q;
    if ((int)a.size() < m) { poly r=a; r.resize(max(0,m-1)); return {r, {}}; }
    q.assign((int)a.size()-m+1, 0);
    poly aa=a; aa.resize(max(aa.size(), (size_t)m));
    for (int i=(int)a.size()-1; i>=m-1; i--) {
        int coef = (LL)aa[i] * ib % p;
        q[i-(m-1)] = coef;
        for (int j=0;j<m;j++) {
            aa[i-(m-1)+j] = (aa[i-(m-1)+j] - (LL)coef * b[j]) % p;
            if (aa[i-(m-1)+j] < 0) aa[i-(m-1)+j] += p;
        }
    }
    poly r(m-1);
    for (int i=0;i<m-1;i++) r[i] = i<(int)aa.size()?aa[i]%p:0;
    return {r, q};
}

mt19937 rng(8);
int fail=0;
void oops(string s){ if(fail<12) cerr<<"FAIL "<<s<<"\n"; fail++; }

poly rand_poly(int n, bool nz_lead) {
    poly a(n);
    for (int i=0;i<n;i++) a[i]=rng()%p;
    if (nz_lead) a.back() = 1+rng()%(p-1);
    return a;
}

void check(const poly& a, const poly& b) {
    auto [r, q] = poly_mod(a, b);
    poly bb=b; while(bb.size()>1 && bb.back()==0) bb.pop_back();
    auto [nr, nq] = naive_div(a, bb);
    poly recon = q.empty() ? poly(a.size()) : naive_mul(bb, q);
    recon.resize(max(recon.size(), a.size()));
    for (int i=0;i<(int)r.size();i++) {
        if (i>=(int)recon.size()) recon.push_back(0);
        recon[i] = (recon[i] + r[i]) % p;
    }
    poly aa=a; aa.resize(recon.size());
    recon.resize(max(recon.size(), aa.size()));
    for (int i=0;i<(int)aa.size();i++) if ((aa[i]%p+p)%p != (recon[i]%p+p)%p) {
        oops("a != bq+r");
        return;
    }
    if ((int)r.size() != max(0, (int)bb.size()-1)) oops("rem size");
    // remainder degree
    if (deg(r) >= deg(bb) && deg(bb)>=0) oops("rem deg");
}

int main() {
    ntt_init(2048);
    // special case {1}/{1,0}
    {
        auto [r,q] = poly_mod({1}, {1,0});
        if (!r.empty() && deg(r)>=0) oops("1/(1,0) rem not 0");
        poly qq=q; while(qq.size()&&qq.back()==0) qq.pop_back();
        if (qq != poly{1} && q != poly{1}) {
            // q may be {1} 
            if (trim(q) != poly{1}) oops("1/(1,0) quot");
        }
    }
    for (int t=0;t<10000;t++) {
        int n=1+t%6, m=1+t%5;
        auto a=rand_poly(n, t%3!=0);
        auto b=rand_poly(m, true);
        if (t%7==0) { b.push_back(0); if (t%2==0) b.push_back(0); }
        check(a,b);
        if (fail>15) break;
    }
    int small_fail=fail;
    for (int t=0;t<1000;t++) {
        int n=8+t%24, m=1+t%12;
        auto a=rand_poly(n, true);
        auto b=rand_poly(m, true);
        if (t%5==0) b.push_back(0);
        check(a,b);
        if (fail>25) break;
    }
    cout << "dp_polymod small_fail=" << small_fail << " total_fail=" << fail << (fail ? " FAIL\n" : " OK\n");
    return fail ? 1 : 0;
}
