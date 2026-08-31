#include <bits/stdc++.h>
using namespace std;
using LL = long long;
using cd = complex<double>;
const int MAXN=2048;
int rev[MAXN];
void fft_init(int n){
    for(int i=0;i<n;i++){ int r=0; for(int b=1,j=i;b<n;b<<=1,j>>=1) r=(r<<1)|(j&1); rev[i]=r; }
}
void fft(cd* a, int n, int sig){
    for(int i=0;i<n;i++) if(i<rev[i]) swap(a[i],a[rev[i]]);
    for(int len=2;len<=n;len<<=1){
        double ang=2*acos(-1)/len*(sig>0?1:-1);
        cd wlen(cos(ang),sin(ang));
        for(int i=0;i<n;i+=len){
            cd w(1);
            for(int j=0;j<len/2;j++){
                cd u=a[i+j], v=a[i+j+len/2]*w;
                a[i+j]=u+v; a[i+j+len/2]=u-v; w*=wlen;
            }
        }
    }
    if(sig<0) for(int i=0;i<n;i++) a[i]/=n;
}
using cp=cd;
void dft(cp* a, cp* b, int n) { static cp c[MAXN];
	for (int i = 0; i < n; i++)
		c[i] = cp(a[i].real(), b[i].real());
	fft(c, n, 1);
	for (int i = 0; i < n; i++) { int j = (n - i) & (n - 1);
		a[i] = (c[i] + conj(c[j])) * 0.5;
		b[i] = (c[i] - conj(c[j])) * cd(0,-0.5); } }
void idft(cp* a, cp* b, int n) { static cp c[MAXN];
	for (int i = 0; i < n; i++) c[i] = a[i] + cd(0,1) * b[i];
	fft(c, n, -1);
	for (int i = 0; i < n; i++) {
		a[i] = c[i].real(); b[i] = c[i].imag(); } }
vector<int> multiply(const vector<int>& u,
		const vector<int>& v, int mod) {
	static cp a[2][MAXN], b[2][MAXN], c[3][MAXN];
	int base = ceil(sqrt(mod));
	int n = (int)u.size(), m = (int)v.size();
	int fft_n = 1; while (fft_n < n + m - 1) fft_n *= 2;
	fft_init(fft_n);
	for (int i = 0; i < 2; i++) {
		fill(a[i], a[i] + fft_n, 0);
		fill(b[i], b[i] + fft_n, 0); }
	for (int i = 0; i < 3; i++)
		fill(c[i], c[i] + fft_n, 0);
	for (int i = 0; i < n; i++) {
		int x = (u[i] % mod + mod) % mod;
		a[0][i] = x % base; a[1][i] = x / base; }
	for (int i = 0; i < m; i++) {
		int x = (v[i] % mod + mod) % mod;
		b[0][i] = x % base; b[1][i] = x / base; }
	dft(a[0], a[1], fft_n); dft(b[0], b[1], fft_n);
	for (int i = 0; i < fft_n; i++) {
		c[0][i] = a[0][i] * b[0][i];
		c[1][i] = a[0][i] * b[1][i] + a[1][i] * b[0][i];
		c[2][i] = a[1][i] * b[1][i]; }
	fft(c[1], fft_n, -1); idft(c[0], c[2], fft_n);
	int base2 = (int)((LL)base * base % mod);
	vector<int> ans(n + m - 1);
	for (int i = 0; i < n + m - 1; i++) {
		LL t = (LL)(c[0][i].real() + 0.5)
			+ (LL)(c[1][i].real() + 0.5) % mod * base
			+ (LL)(c[2][i].real() + 0.5) % mod * base2;
		ans[i] = (int)((t % mod + mod) % mod); }
	return ans; }
vector<int> brute(const vector<int>& u, const vector<int>& v, int mod){
    int n=u.size(), m=v.size();
    vector<int> c(n+m-1);
    for(int i=0;i<n;i++) for(int j=0;j<m;j++)
        c[i+j]=(c[i+j]+(LL)((u[i]%mod+mod)%mod)*((v[j]%mod+mod)%mod))%mod;
    return c;
}
mt19937 rng(122);
int fail=0;
int main(){
    for(int t=0;t<10000;t++){
        int n=1+t%4, m=1+t%4, mod=1+rng()%1000+2;
        vector<int> u(n), v(m);
        for(int i=0;i<n;i++) u[i]=(int)rng()%2000-1000;
        for(int i=0;i<m;i++) v[i]=(int)rng()%2000-1000;
        auto g=multiply(u,v,mod), b=brute(u,v,mod);
        if(g!=b){ fail++; if(fail<5) cerr<<"mtt\n"; }
    }
    int sf=fail;
    for(int t=0;t<1000;t++){
        int n=4+t%8, m=4+t%8, mod=1000000007;
        vector<int> u(n), v(m);
        for(int i=0;i<n;i++) u[i]=(int)rng()%mod-mod/2;
        for(int i=0;i<m;i++) v[i]=(int)rng()%mod-mod/2;
        auto g=multiply(u,v,mod), b=brute(u,v,mod);
        if(g!=b) fail++;
    }
    cout<<"dp_mtt small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}