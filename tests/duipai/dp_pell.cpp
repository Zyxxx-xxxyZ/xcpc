#include <bits/stdc++.h>
using namespace std;
using LL = long long;
const int N = 2000;
pair<LL, LL> peLL(LL n) {
	static __int128 p[N], q[N], g[N], h[N], a[N];
	if (n <= 0) return {0, 0};
	p[1] = q[0] = h[1] = 1; p[0] = q[1] = g[1] = 0;
	__int128 s = (LL)floor(sqrtl((long double)n));
	while ((s + 1) * (s + 1) <= n) ++s;
	while (s * s > n) --s;
	if (s * s == n) return {0, 0};
	a[2] = s;
	for(int i = 2; i < N - 2; i ++) {
		if (!h[i - 1]) return {0, 0};
		g[i] = -g[i - 1] + a[i] * h[i - 1];
		h[i] = (n - g[i] * g[i]) / h[i - 1];
		if (!h[i]) return {0, 0};
		a[i + 1] = (g[i] + a[2]) / h[i];
		p[i] = a[i] * p[i - 1] + p[i - 2];
		q[i] = a[i] * q[i - 1] + q[i - 2];
		if (p[i] < 0 || q[i] < 0 || p[i] > LLONG_MAX || q[i] > LLONG_MAX) return {0, 0};
		if(p[i] * p[i] - (__int128)n * q[i] * q[i] == 1)
			return {(LL)p[i], (LL)q[i]};
	}
	return {0, 0}; }
pair<LL,LL> brute(LL n, LL ylim){
    for(LL y=1;y<=ylim;y++){
        __int128 t=1+(__int128)n*y*y;
        LL x=sqrt((long double)t);
        while((__int128)x*x<t) x++;
        while((__int128)x*x>t) x--;
        if((__int128)x*x==t) return {x,y};
    }
    return {0,0};
}
int fail=0;
void oops(string s){ if(fail<8) cerr<<"FAIL "<<s<<"\n"; fail++; }
int main(){
    for(LL n=1;n<=10000;n++){
        auto g=peLL(n);
        LL sq=(LL)sqrt((long double)n); while(sq*sq<n)sq++; while(sq*sq>n)sq--;
        if(sq*sq==n){
            if(g.first||g.second) oops("square n="+to_string(n));
            continue;
        }
        if(g.first==0){
            auto b=brute(n, 2000);
            if(b.first) oops("miss n="+to_string(n));
            continue;
        }
        if((__int128)g.first*g.first - (__int128)n*g.second*g.second != 1)
            oops("eq n="+to_string(n));
        if(g.second <= 8000){
            auto b=brute(n, g.second);
            if(b.first && (b.second < g.second || (b.second==g.second && b.first<g.first)))
                oops("not min n="+to_string(n));
            if(!b.first) oops("brute miss fund n="+to_string(n));
        } else {
            auto b=brute(n, 8000);
            if(b.first) oops("not min (small y) n="+to_string(n));
        }
    }
    int sf=fail;
    for(int t=0;t<1000;t++){
        LL n=10001 + t*97;
        auto g=peLL(n);
        LL sq=(LL)sqrt((long double)n); while((__int128)sq*sq<n)sq++; while((__int128)sq*sq>n)sq--;
        if((__int128)sq*sq==n){ if(g.first||g.second) oops("sq large"); continue; }
        if(g.first==0) continue; // overflow / period too long
        if((__int128)g.first*g.first - (__int128)n*g.second*g.second != 1)
            oops("large eq n="+to_string(n));
        if(g.first<=0 || g.second<=0) oops("sign");
    }
    cout<<"dp_pell small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}
