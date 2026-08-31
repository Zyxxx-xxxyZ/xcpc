#include <bits/stdc++.h>
using namespace std;
const int INF = 1e9;
int M, t[4096], mark[4096];
void update(int l, int r, int d) {
	for (l += M-1, r += M+1; l^r^1; l >>= 1, r >>= 1) {
		if (l < M) {
			t[l] = max(t[l*2], t[l*2+1]) + mark[l];
			t[r] = max(t[r*2], t[r*2+1]) + mark[r]; }
		if (~l & 1) { t[l ^ 1] += d; mark[l ^ 1] += d; }
		if (r & 1) { t[r ^ 1] += d; mark[r ^ 1] += d; } }
	for (; l; l >>= 1, r >>= 1)
		if (l < M) t[l] = max(t[l*2], t[l*2+1]) + mark[l],
				   t[r] = max(t[r*2], t[r*2+1]) + mark[r]; }
int query(int l, int r) {
	int maxl = -INF, maxr = -INF;
	for (l += M-1, r += M+1; l^r^1; l >>= 1, r >>= 1) {
		maxl += mark[l]; maxr += mark[r];
		if (~l & 1) maxl = max(maxl, t[l ^ 1]);
		if ( r & 1) maxr = max(maxr, t[r ^ 1]); }
	while (l) { maxl += mark[l]; maxr += mark[r];
		l >>= 1; r >>= 1; }
	return max(maxl, maxr); }
mt19937 rng(105);
int fail=0;
int main(){
    auto run=[&](int n, int nops){
        M=1; while(M<n+2) M<<=1;
        memset(t,0,sizeof t); memset(mark,0,sizeof mark);
        vector<int> a(n+1);
        for(int i=0;i<nops;i++){
            int l=1+rng()%n, r=1+rng()%n; if(l>r) swap(l,r);
            if(rng()%2==0){
                int d=(int)rng()%7-3;
                update(l,r,d);
                for(int j=l;j<=r;j++) a[j]+=d;
            } else {
                int g=query(l,r);
                int bf=-INF; for(int j=l;j<=r;j++) bf=max(bf,a[j]);
                if(g!=bf){ fail++; if(fail<6) cerr<<"q "<<l<<","<<r<<" got="<<g<<" bf="<<bf<<"\n"; }
            }
        }
    };
    for(int t=0;t<10000;t++) run(4+t%8, 12+t%10);
    int sf=fail;
    for(int t=0;t<1000;t++) run(20+t%40, 40+t%40);
    cout<<"dp_segmax small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}