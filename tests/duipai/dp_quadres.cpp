#include <bits/stdc++.h>
using namespace std;
using ll = long long;
int MOD;
ll power(ll a, ll k, ll m){ ll r=1; a%=m; for(;k;k>>=1,a=a*a%m) if(k&1) r=r*a%m; return r; }
void multiply(ll &c, ll &d, ll a, ll b, ll w) {
	int cc = (a * c + b * d % MOD * w) % MOD;
	int dd = (a * d + b * c) % MOD; c = cc, d = dd; }
bool solve(int n, int &x) {
	n %= MOD; if (n < 0) n += MOD;
	if (n==0) return x=0,true; if (MOD==2) return x=1,true;
	if (power(n, MOD / 2, MOD) == MOD - 1) return false;
	ll c = 1, d = 0, b = 1, a, w;
	do { a = rand() % MOD; w = (a * a - n + MOD) % MOD;
		if (w == 0) return x = a, true;
	} while (power(w, MOD / 2, MOD) != MOD - 1);
	for (int times = (MOD + 1) / 2; times; times >>= 1) {
		if (times & 1) multiply(c, d, a, b, w);
		multiply(a, b, a, b, w); }
	return x = c, true; }
int fail=0;
int main(){
    srand(14);
    vector<int> mods = {3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,97,101,251,257};
    int t=0;
    for(;t<10000;){
        MOD = mods[t % mods.size()];
        int n = t % MOD;
        int x=-1; bool ok=solve(n,x);
        bool exists=false; int bf=-1;
        for(int i=0;i<MOD;i++) if((ll)i*i%MOD==n){ exists=true; bf=i; break; }
        if(ok!=exists){ fail++; if(fail<8) cerr<<"exist MOD="<<MOD<<" n="<<n<<"\n"; }
        else if(ok && (ll)x*x%MOD != n){ fail++; if(fail<8) cerr<<"wrong sqrt\n"; }
        t++;
    }
    int sf=fail;
    srand(99);
    for(int k=0;k<1000;k++){
        MOD=1000000007;
        int n = rand()%MOD;
        int x=-1; bool ok=solve(n,x);
        ll leg = power(n, (MOD-1)/2, MOD);
        bool exists = (n==0) || leg==1;
        if(ok!=exists){ fail++; }
        else if(ok && (ll)x*x%MOD != n) fail++;
    }
    cout<<"dp_quadres small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}
