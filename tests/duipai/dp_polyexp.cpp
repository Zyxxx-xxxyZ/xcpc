#include <bits/stdc++.h>
using namespace std;
using LL = long long;
using ULL = unsigned long long;
using poly = vector<int>;
const int p = 998244353, inv_2 = (p+1)/2;
const int MAXN = 2048, N=2048;
int qpow(int a, int k){ int r=1; for(;k;k>>=1,a=(LL)a*a%p) if(k&1) r=(LL)r*a%p; return r; }
int invv[MAXN];
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
poly poly_mul(const poly& u, const poly& v) {
	return poly_calc(u, v, [](int a, int b){ return (LL)a * b % p; }); }
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
poly poly_sqrt(const poly& a) {
	int n = (int)a.size(), NN = 1; while (NN < n) NN *= 2;
	poly aa = a; aa.resize(NN);
	poly c{1};
	for (int k = 2; k <= NN; k *= 2) {
		c.resize(k); poly b(aa.begin(), aa.begin() + k);
		b = poly_mul(b, poly_inv(c));
		for (int i = 0; i < k; i++)
			c[i] = (LL)(c[i] + b[i]) * inv_2 % p; }
	c.resize(n); return c; }
poly poly_derivative(const poly& a) { poly c(a.size());
	for (int i = 1; i < (int)a.size(); i++)
		c[i - 1] = (LL)a[i] * i % p; return c; }
poly poly_integrate(const poly& a) { poly c(a.size());
	for (int i = 1; i < (int)a.size(); i++)
		c[i] = (LL)a[i - 1] * invv[i] % p; return c; }
poly poly_ln(const poly& a) {
	auto c = poly_mul(poly_derivative(a), poly_inv(a));
	c.resize(a.size()); return poly_integrate(c); }
poly poly_exp(const poly& a) {
	int n = (int)a.size(), NN = 1; while (NN < n) NN *= 2;
	poly aa = a; aa.resize(NN); poly c{1};
	for (int k = 2; k <= NN; k *= 2) {
		c.resize(k); auto b = poly_ln(c);
		for (int i = 0; i < k; i++)
			b[i] = (aa[i] - b[i] + p) % p;
		(++b[0]) %= p; c = poly_mul(b, c);
		memset(c.data() + k, 0, sizeof(int) * k); }
	c.resize(n); return c; }
poly naive_mul_mod(poly a, poly b, int n){
    poly c(n);
    for(int i=0;i<(int)a.size();i++) for(int j=0;j<(int)b.size() && i+j<n;j++)
        c[i+j]=(c[i+j]+(LL)a[i]*b[j])%p;
    return c;
}
mt19937 rng(126);
int fail=0;
int main(){
    ntt_init(1024);
    invv[1]=1; for(int i=2;i<MAXN;i++) invv[i]=(LL)(p-p/i)*invv[p%i]%p;
    for(int t=0;t<10000;t++){
        int n=1+t%6;
        poly a(n); a[0]=1; for(int i=1;i<n;i++) a[i]=rng()%p;
        auto s=poly_sqrt(a);
        auto ss=naive_mul_mod(s,s,n);
        if(ss!=a){ fail++; if(fail<5) cerr<<"sqrt n="<<n<<"\n"; }
        poly e(n); e[0]=0; for(int i=1;i<n;i++) e[i]=rng()%p;
        auto ex=poly_exp(e);
        auto ln=poly_ln(ex);
        for(int i=0;i<n;i++) if(ln[i]!=e[i]) { fail++; break; }
    }
    int sf=fail;
    for(int t=0;t<1000;t++){
        int n=8+t%8;
        poly a(n); a[0]=1; for(int i=1;i<n;i++) a[i]=rng()%p;
        auto s=poly_sqrt(a);
        if(naive_mul_mod(s,s,n)!=a) fail++;
        poly e(n); e[0]=0; for(int i=1;i<n;i++) e[i]=rng()%p;
        auto ln=poly_ln(poly_exp(e));
        if(ln!=e) fail++;
    }
    cout<<"dp_polyexp small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}