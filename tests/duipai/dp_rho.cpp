#include <bits/stdc++.h>
using namespace std;
using LL = long long;
LL mul(LL a, LL b, LL m){ return (LL)((__int128)a*b%m); }
LL qpow(LL a, LL k, LL m){ LL r=1%m; for(;k;k>>=1,a=mul(a,a,m)) if(k&1) r=mul(r,a,m); return r; }
mt19937_64 rng_real(114);
#define rand() (LL)(rng_real() & LLONG_MAX)
const LL BASE[] = {2,325,9375,28178,450775,9780504,1795265022};
struct miller_rabin {
bool check (LL M, LL base) {
	LL a = M - 1;
	while (~a & 1) a >>= 1;
	LL w = qpow (base, a, M);
	for (; a != M - 1 && w != 1 && w != M - 1; a <<= 1)
		w = mul (w, w, M);
	return w == M - 1 || (a & 1) == 1; }
bool solve (LL a) {
	if (a < 4) return a > 1;
	if (~a & 1) return false;
	for (int i = 0; i < (int)(sizeof(BASE)/sizeof(BASE[0])); ++i) {
        LL chk = BASE[i] % a;
        if (chk == 0) continue;
		if (!check (a, chk)) return false;
    }
	return true; } };
miller_rabin is_prime;
LL get_factor (LL a, LL seed) {
	LL x = rand() % (a - 1) + 1, y = x;
	for (int head = 1, tail = 2; ; ) {
		x = mul (x, x, a); x = (x + seed) % a;
		if (x == y) return a;
		LL ans = gcd (abs (x - y), a);
		if (ans > 1 && ans < a) return ans;
		if (++head == tail) { y = x; tail <<= 1; } } }
void factor (LL a, vector<LL> &d) {
	if (a <= 1) return;
	if (is_prime.solve (a)) d.push_back (a);
	else {
		LL f = a;
		for (; f >= a; f = get_factor (a, rand() % (a - 1) + 1));
		factor (a / f, d);
		factor (f, d); } }
int fail=0;
int main(){
    for(LL n=2;n<=10000;n++){
        vector<LL> d; factor(n,d);
        LL p=1; for(auto x:d) p*=x;
        if(p!=n){ fail++; if(fail<5) cerr<<"prod n="<<n<<"\n"; }
        for(auto x:d) if(!is_prime.solve(x)) fail++;
    }
    int sf=fail;
    mt19937_64 rng(3);
    for(int t=0;t<1000;t++){
        LL n=2+rng()%200000;
        vector<LL> d; factor(n,d);
        LL p=1; for(auto x:d){
            if(p>LLONG_MAX/x){ p=0; break; }
            p*=x;
        }
        if(p!=n) fail++;
        for(auto x:d) if(x<=1 || n%x) fail++;
    }
    cout<<"dp_rho small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}