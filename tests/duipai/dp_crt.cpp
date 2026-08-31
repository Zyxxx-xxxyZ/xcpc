#include <bits/stdc++.h>
#include <cassert>
using namespace std;
using LL = long long;
LL exgcd(LL a, LL b, LL &x, LL &y) {
	if (b == 0) return x = 1, y = 0, a;
	LL t = exgcd(b, a % b, y, x);
	y -= a / b * x; return t;}
LL inv(LL x, LL m) {
	LL a, b; LL g = exgcd(x, m, a, b);
	assert(g == 1 || g == -1);
	return (a % m + m) % m; }
bool crt_merge(LL a1, LL m1, LL a2, LL m2, LL &A, LL &M) {
LL c = a2 - a1, d = __gcd(m1, m2);
if(c % d) return 0;
c = (c % m2 + m2) % m2; c /= d; m1 /= d; m2 /= d;
c = c * inv(m1 % m2, m2) % m2;
M = m1*m2*d; A = (c *m1 %M *d %M +a1) % M;
	if (A < 0) A += M; return 1;}
mt19937_64 rng(10);
int fail=0;
void oops(string s){ if(fail<8) cerr<<"FAIL "<<s<<"\n"; fail++; }
int main(){
    for(int t=0;t<10000;t++){
        LL m1=2+rng()%40, m2=2+rng()%40;
        LL a1=rng()%m1, a2=rng()%m2;
        LL A,M; bool ok=crt_merge(a1,m1,a2,m2,A,M);
        bool exists=false; LL sol=-1;
        LL lim=m1/__gcd(m1,m2)*m2;
        for(LL x=0;x<lim;x++) if(x%m1==a1 && x%m2==a2){ exists=true; sol=x; break; }
        if(ok!=exists) oops("exist");
        else if(ok && !(A%m1==a1 && A%m2==a2 && M==lim && A>=0 && A<M)) oops("value");
    }
    int sf=fail;
    for(int t=0;t<1000;t++){
        LL m1=2+rng()%10000, m2=2+rng()%10000;
        LL a1=rng()%m1, a2=rng()%m2;
        LL A,M; bool ok=crt_merge(a1,m1,a2,m2,A,M);
        LL d=__gcd(m1,m2);
        bool exists=((a2-a1)%d==0);
        if(ok!=exists) oops("large exist");
        else if(ok){
            if(A<0 || A>=M || A%m1!=a1 || A%m2!=a2) oops("large val");
            if(M != m1/d*m2) oops("M");
        }
    }
    // inv check
    for(int t=0;t<10000;t++){
        LL m=2+rng()%1000; LL x=rng()%m; if(__gcd(x,m)!=1) continue;
        LL i=inv(x,m);
        if(((__int128)i*x)%m != 1%m && ((__int128)i*x)%m != 1) {
            if(m==1) continue;
            if(((__int128)x*i)%m != 1) oops("inv");
        }
    }
    cout<<"dp_crt small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}
