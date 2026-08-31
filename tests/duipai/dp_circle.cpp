#include <bits/stdc++.h>
using namespace std;
using LL = long long;
vector <LL> solve(LL r) {
	vector <LL> ret;
	ret.push_back(0);
	LL l = 2 * r, s = sqrt(l);
	for (LL d=1; d<=s; d++) if (l%d==0) {
		LL lim=LL(sqrt(l/(2*d)));
		for (LL a = 1; a <= lim; a++) {
			LL b = sqrt(l/d-a*a);
			if (a*a+b*b==l/d && __gcd(a,b)==1 && a!=b)
				ret.push_back(d*a*b);
		} if (d*d==l) break;
		lim = sqrt(d/2);
		for (LL a=1; a<=lim; a++) {
			LL b = sqrt(d - a * a);
			if (a*a+b*b==d && __gcd(a,b)==1 && a!=b)
				ret.push_back(l/d*a*b);
	} } ret.push_back(r); return ret; }
set<LL> brute_y(LL r){
    set<LL> s;
    for(LL y=0;y<=r;y++){
        LL t=r*r-y*y; LL x=sqrt((long double)t);
        while(x*x<t)x++; while(x*x>t && x)x--;
        if(x*x==t) s.insert(y);
    }
    return s;
}
int fail=0;
int main(){
    for(LL r=1;r<=10000;r++){
        auto v=solve(r);
        set<LL> g(v.begin(), v.end());
        auto b=brute_y(r);
        if(g!=b){ fail++; if(fail<8){
            cerr<<"r="<<r<<" got"; for(auto x:g) cerr<<" "<<x;
            cerr<<" bf"; for(auto x:b) cerr<<" "<<x; cerr<<"\n";
        }}
        if(fail>15) break;
    }
    int sf=fail;
    for(int t=0;t<1000;t++){
        LL r=10001 + t*17;
        auto v=solve(r);
        set<LL> g(v.begin(), v.end());
        auto b=brute_y(r);
        if(g!=b) fail++;
    }
    cout<<"dp_circle small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}
