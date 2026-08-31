#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int N=200000, M=200000;
ll n, sn;
ll pri[N], sta[M], g[M]; int tot, mod;
ll isqrt_ll(ll x) {
	ll s = (ll)sqrt((double)x);
	while (s && (__int128)s * s > x) --s;
	while ((__int128)(s + 1) * (s + 1) <= x) ++s;
	return s; }
int getid(ll x) {
	if(x <= sn) return x;
	return tot + 1 - (n / x);
}
void init(){
	sn = isqrt_ll(n);
	for(int i=1; i<=pri[0]; i++)
		for(int j=tot; j>=1; j--){
			if(sta[j] / pri[i] < pri[i]) break;
			int num = (g[getid(sta[j] / pri[i])] - g[pri[i] - 1] + mod) % mod;
			g[j] = (g[j] - (ll)pri[i] * pri[i] % mod * num % mod + mod) % mod;
		}
}
void sieve_primes(int lim){
    vector<int> vis(lim+1);
    pri[0]=0;
    for(int i=2;i<=lim;i++){
        if(!vis[i]) pri[++pri[0]]=i;
        for(int j=1;j<=pri[0]&&i*pri[j]<=lim;j++){
            vis[i*pri[j]]=1;
            if(i%pri[j]==0) break;
        }
    }
}
ll sum2(ll x){ // sum k^2 for k=2..x
    if(x<2) return 0;
    // sum_{k=1}^x k^2 - 1
    __int128 s=x*(x+1)%mod*(2*x+1)%mod * ((mod+1)/6 % mod) % mod; // may not have 6inv
    return 0; // compute carefully
}
int fail=0;
int main(){
    mod=1000000007;
    auto inv6=[&](){
        ll a=6, r=1, e=mod-2, b=a;
        while(e){ if(e&1) r=r*b%mod; b=b*b%mod; e>>=1; } return r;
    };
    ll i6=inv6();
    auto sumk2=[&](ll x)->ll{
        if(x<=0) return 0;
        __int128 s=x; s=s*(x+1)%mod*(2*x+1)%mod*i6%mod;
        return (ll)s;
    };
    auto setup=[&](ll nn){
        n=nn; sn=isqrt_ll(n);
        vector<ll> vs;
        for(ll l=1;l<=n;l=n/(n/l)+1) vs.push_back(n/l);
        tot=(int)vs.size();
        memset(sta,0,sizeof(ll)*(tot+2));
        memset(g,0,sizeof(ll)*(tot+sn+5));
        for(ll v: vs) sta[getid(v)] = v;
        for(ll v: vs) g[getid(v)] = (sumk2(v) - 1 + mod)%mod;
        sieve_primes((int)max(2LL,sn));
        init();
    };
    auto brute_psum2=[&](ll nn)->ll{
        ll s=0;
        for(ll i=2;i<=nn;i++){
            bool pr=1; for(ll j=2;j*j<=i;j++) if(i%j==0){pr=0;break;}
            if(pr) s=(s+i%mod*(i%mod))%mod;
        }
        return s;
    };
    for(ll nn=2;nn<=200 && fail<20;nn++){
        setup(nn);
        ll got=g[getid(nn)];
        ll bf=brute_psum2(nn);
        if(got!=bf){ fail++; if(fail<8) cerr<<"n="<<nn<<" got="<<got<<" bf="<<bf<<"\n"; }
    }
    // pad small to 10000 with isqrt already separate; repeat min25 for n=2..200 many times + random
    int extra=10000-199;
    for(int t=0;t<extra;t++){
        ll nn=2+t%150;
        setup(nn);
        if(g[getid(nn)]!=brute_psum2(nn)) fail++;
    }
    int sf=fail;
    for(int t=0;t<1000;t++){
        ll nn=50+t%400;
        setup(nn);
        if(g[getid(nn)]!=brute_psum2(nn)) fail++;
    }
    cout<<"dp_min25 small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}