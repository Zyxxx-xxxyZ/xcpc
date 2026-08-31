#include <bits/stdc++.h>
using namespace std;
using LL = long long;
using ULL = unsigned long long;
using u128 = unsigned __int128;
LL modmul(LL a, LL b, LL M) {
	LL ret = (LL)((__int128)a * b % M);
	return ret < 0 ? ret + M : ret; }
ULL modmul(ULL a, ULL b, LL M) {
	ULL m = (ULL)M;
	return (ULL)((unsigned __int128)a * b % m); }
struct DIV {
	ULL p, ip;
	void init (ULL _p) { p = _p; ip = -1llu / p; }
	ULL mod (ULL x) {
		ULL q = ULL(((u128)ip * x) >> 64);
		ULL r = x - q * p;
		return r >= p ? r - p : r;
} };
mt19937_64 rng(101);
int fail=0;
void oops(string s){ if(fail<8) cerr<<"FAIL "<<s<<"\n"; fail++; }
int main(){
    for(int t=0;t<10000;t++){
        LL M = 2 + (LL)(rng()%((1ULL<<62)-3));
        LL a = (LL)rng() % M, b = (LL)rng() % M;
        if(a<0) a=-a; if(b<0) b=-b;
        LL g = modmul(a,b,M);
        LL bf = (LL)(((__int128)(a%M+M)%M * ((b%M+M)%M)) % M);
        if(g!=bf) oops("ll mul");
        ULL ua=rng(), ub=rng();
        ULL ug=modmul(ua,ub,M);
        ULL ubf=(ULL)((u128)ua*ub%(ULL)M);
        if(ug!=ubf) oops("ull mul");
        DIV d; d.init((ULL)M);
        ULL x=rng();
        if(d.mod(x) != x%(ULL)M) oops("div");
    }
    int sf=fail;
    for(int t=0;t<1000;t++){
        LL M = (1LL<<62) - 1 - (t%1000);
        if(M<2) M=3;
        for(int k=0;k<20;k++){
            LL a=rng()%(M), b=rng()%M;
            if(modmul(a,b,M) != (LL)((__int128)a*b%M)) fail++;
        }
        DIV d; d.init((ULL)M);
        for(int k=0;k<20;k++){
            ULL x=rng();
            if(d.mod(x)!=x%(ULL)M) fail++;
        }
    }
    cout<<"dp_llfpm small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}
