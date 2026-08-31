#include <bits/stdc++.h>
using namespace std;
using LL = long long;
using Val=unsigned long long;
typedef vector<Val> poly;
poly polymul(const poly &a,const poly &b, Val mod){
	int n = (int) a.size(); poly c (n, Val(0));
	for (int i = 0; i < n; ++ i) {
		for (int j = 0; i + j < n; ++ j) {
			c[i + j] = (Val)((unsigned __int128)(c[i + j] + (unsigned __int128)a[i] * b[j]) % mod); } }
	return c; } Val choo[70][70];
poly polyshift(const poly &a, Val delta, Val mod) {
	int n = (int) a.size(); poly res (n, Val(0));
	for (int i = 0; i < n; ++ i) { Val d = 1;
		for (int j = 0; j <= i; ++ j) {
			res[i - j] = (Val)((unsigned __int128)(res[i - j] + (unsigned __int128)a[i]*choo[i][j]%mod*d) % mod);
			d = (Val)((unsigned __int128)d * delta % mod); } } return res; }
void prepare(int q) {
	for (int i = 0; i < q; ++ i) { choo[i][0] = Val(1);
		for (int j = 1; j <= i; ++ j)
			choo[i][j]=choo[i-1][j-1]+choo[i-1][j]; } }
pair<Val, LL> fact(LL n, LL p, LL q) { prepare((int)q);
	Val mod = 1; for (int i = 0; i < q; ++i) mod *= (Val)p; Val ans = 1;
	for (int r = 1; r < p; ++ r) {
		poly x (q, Val(0)), res (q, Val(0));
		res[0] = 1; LL _res = 0; x[0] = r; LL _x = 0;
		if (q > 1) x[1] = p, _x = 1; LL m = (n - r + p) / p;
		while (m) { if (m & 1) {
				res=polymul(res,polyshift(x,_res,mod),mod); _res+=_x; }
			m >>= 1; x = polymul(x, polyshift(x, _x, mod), mod); _x+=_x; }
		ans = (Val)((unsigned __int128)ans * res[0] % mod); }
	LL cnt = n / p; if (n >= p) { auto tmp=fact(n / p, p, q);
		ans = (Val)((unsigned __int128)ans * tmp.first % mod); cnt += tmp.second; }
	return {ans, cnt}; }
pair<Val,LL> brute(LL n, LL p, LL q){
    Val mod=1; for(int i=0;i<q;i++) mod*=(Val)p;
    Val a=1; LL b=0;
    for(LL i=1;i<=n;i++){
        LL x=i;
        while(x%p==0){ x/=p; b++; }
        a = (Val)((unsigned __int128)a * (x%mod) % mod);
    }
    return {a,b};
}
int fail=0;
int main(){
    int t=0;
    for(LL n=0;n<=80;n++) for(LL p:{2,3,5,7}) for(LL q=1;q<=3;q++){
        t++;
        auto g=fact(n,p,q), b=brute(n,p,q);
        if(g!=b){ fail++; if(fail<8) cerr<<"n="<<n<<" p="<<p<<" q="<<q
            <<" got "<<g.first<<","<<g.second<<" bf "<<b.first<<","<<b.second<<"\n"; }
        if(t>=10000) goto done_small;
    }
    // pad to 10000 with random small
    while(t<10000){
        LL n=t%50, p=2+(t%4==0?1:0); if(p==2||p==3||p==5||p==7);
        p = (t%4==0)?2:(t%4==1)?3:(t%4==2)?5:7;
        LL q=1+t%3;
        if(fact(n,p,q)!=brute(n,p,q)) fail++;
        t++;
    }
done_small:;
    int sf=fail;
    for(int k=0;k<1000;k++){
        LL n=20+k%80, p=(k%3==0)?2:(k%3==1)?3:5, q=1+k%3;
        if(fact(n,p,q)!=brute(n,p,q)) fail++;
    }
    cout<<"dp_factmod small_fail="<<sf<<" total_fail="<<fail<<" cases_small="<<t<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}