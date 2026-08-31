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
	else { int inv = qpow(n, p - 2); for (int i = 0; i < n; i++) c[i] = (int)(a[i]%p * inv % p);}}
poly poly_calc(const poly& u, const poly& v, function<int(int, int)> op) {
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
poly naive_inv(poly a){
    int n=a.size(); poly b(n); b[0]=qpow(a[0], p-2);
    for(int i=1;i<n;i++){
        LL s=0; for(int j=1;j<=i;j++) s+= (LL)a[j]*b[i-j];
        b[i] = (p - (s%p)*b[0]%p) % p;
    }
    return b;
}
mt19937 rng(21);
int fail=0;
int main(){
    ntt_init(2048);
    for(int t=0;t<10000;t++){
        int n=1+t%8;
        poly a(n); a[0]=1+rng()%(p-1); for(int i=1;i<n;i++) a[i]=rng()%p;
        auto g=poly_inv(a), b=naive_inv(a);
        if(g!=b){ fail++; if(fail<5) cerr<<"inv n="<<n<<"\n"; }
    }
    int sf=fail;
    for(int t=0;t<1000;t++){
        int n=16+t%48;
        poly a(n); a[0]=1+rng()%(p-1); for(int i=1;i<n;i++) a[i]=rng()%p;
        auto g=poly_inv(a);
        // check a*g = 1 mod x^n naive first n
        vector<int> c(n);
        for(int i=0;i<n;i++) for(int j=0;j<n-i;j++) c[i+j]=(c[i+j]+(LL)a[i]*g[j])%p;
        if(c[0]!=1) fail++;
        for(int i=1;i<n;i++) if(c[i]) { fail++; break; }
    }
    cout<<"dp_polyinv small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}
