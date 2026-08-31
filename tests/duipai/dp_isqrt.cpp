#include <bits/stdc++.h>
using namespace std;
using ll = long long;
ll isqrt_ll(ll x) {
	ll s = (ll)sqrt((double)x);
	while (s && (__int128)s * s > x) --s;
	while ((__int128)(s + 1) * (s + 1) <= x) ++s;
	return s; }
mt19937_64 rng(107);
int fail=0;
ll brute_isqrt(ll x){
    if(x<0) return 0;
    ll s=0, e=min(x,(ll)3e9);
    while(s<e){ ll m=s+e+1>>1; if((__int128)m*m<=x) s=m; else e=m-1; }
    return s;
}
int main(){
    for(int t=0;t<10000;t++){
        ll x=t; // 0..9999 exhaustive
        if(isqrt_ll(x)!=brute_isqrt(x)) fail++;
        x=rng()%1000000000ull;
        if(isqrt_ll(x)!=brute_isqrt(x)) fail++;
    }
    int sf=fail;
    for(int t=0;t<1000;t++){
        ll x=rng()>>1; // 63-bit nonneg
        ll s=isqrt_ll(x);
        if(!(s>=0 && (__int128)s*s<=x && (s==LLONG_MAX || (__int128)(s+1)*(s+1)>x))) fail++;
    }
    // near squares
    for(ll k=1;k<=1000;k++){
        ll sq=k*k;
        if(isqrt_ll(sq)!=k) fail++;
        if(k>1 && isqrt_ll(sq-1)!=k-1) fail++;
    }
    cout<<"dp_isqrt small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}