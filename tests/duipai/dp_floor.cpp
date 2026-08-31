#include <bits/stdc++.h>
#include <cassert>
using namespace std;
using LL = long long;
LL solve(LL n, LL a, LL b, LL m){
	assert(n >= 0 && m > 0 && a >= 0 && b >= 0);
	if (b == 0) return (LL)((__int128)n * (a / m));
	if (a >= m) return (LL)((__int128)n * (a / m) + solve(n, a % m, b, m));
	if (b >= m) return (LL)((__int128)(n-1)*n/2*(b/m) + solve(n,a,b%m,m));
	return solve((LL)(((__int128)a + (__int128)b * n) / m),
		(LL)(((__int128)a + (__int128)b * n) % m), m, b); }
LL brute(LL n, LL a, LL b, LL m){
    LL s=0; for(LL i=0;i<n;i++) s += (LL)(((__int128)a+(__int128)b*i)/m); return s;
}
mt19937_64 rng(9);
int fail=0;
int main(){
    for(int t=0;t<10000;t++){
        LL n=rng()%80, a=rng()%50, b=rng()%50, m=1+rng()%50;
        if(solve(n,a,b,m)!=brute(n,a,b,m)) { fail++; if(fail<5) cerr<<"small\n"; }
    }
    int sf=fail;
    for(int t=0;t<1000;t++){
        LL n=200+rng()%1500, a=rng()%100000, b=rng()%100000, m=1+rng()%100000;
        LL bf=brute(n,a,b,m), got=solve(n,a,b,m);
        if(got!=bf) { fail++; if(fail<8) cerr<<"large "<<n<<" "<<a<<" "<<b<<" "<<m<<"\n"; }
    }
    cout<<"dp_floor small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}
