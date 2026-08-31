#include <bits/stdc++.h>
using namespace std;
using LL = long long;
LL mul(LL a, LL b, LL m){ return (LL)((__int128)a*b%m); }
LL qpow(LL a, LL k, LL m){ LL r=1%m; for(;k;k>>=1,a=mul(a,a,m)) if(k&1) r=mul(r,a,m); return r; }
mt19937_64 rng_real(13);
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
bool trial(LL n){
    if(n<2) return false;
    for(LL i=2;i*i<=n;i++) if(n%i==0) return false;
    return true;
}
int fail=0;
int main(){
    miller_rabin is_prime;
    for(int t=0;t<10000;t++){
        LL n=t+1; // 1..10000 exhaustive small
        if(n<=10000){
            bool a=is_prime.solve(n), b=trial(n);
            if(a!=b){ fail++; if(fail<8) cerr<<"n="<<n<<" mr="<<a<<" tr="<<b<<"\n"; }
        }
    }
    int sf=fail;
    // large 1000: random 32-bit vs trial up to sqrt ~ 2^16
    for(int t=0;t<1000;t++){
        LL n=2 + rng_real()%2000000ull;
        bool a=is_prime.solve(n), b=trial(n);
        if(a!=b){ fail++; if(fail<8) cerr<<"large n="<<n<<"\n"; }
    }
    // known composites in BASE
    for (LL x : {325LL,9375LL,28178LL}) if (is_prime.solve(x)) { fail++; cerr<<"BASE composite accepted "<<x<<"\n"; }
    cout<<"dp_miller small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}
