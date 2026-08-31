#include <bits/stdc++.h>
using namespace std;
const int MAXN = 128;
int siz_global;
template<int len = 1>
int solve_cnt(const vector<int>& w) {
	if((int)w.size() > 0 && siz_global > len && len < MAXN) return solve_cnt<min(len*2, MAXN)>(w);
	bitset<len> dp; dp[0]=1;
	for(int x: w) if(x>=0 && x<len) dp |= dp << x;
	int c=0; for(int i=0;i<len;i++) c+=dp[i];
	return c;
}
int brute(const vector<int>& w, int cap){
    bitset<128> dp; dp[0]=1;
    for(int x:w) if(x>=0&&x<cap) dp|=dp<<x;
    int c=0; for(int i=0;i<cap;i++) c+=dp[i];
    return c;
}
mt19937 rng(124);
int fail=0;
int main(){
    for(int t=0;t<10000;t++){
        int n=1+t%6; vector<int> w(n);
        int sum=0;
        for(int i=0;i<n;i++){ w[i]=1+rng()%6; sum+=w[i]; }
        siz_global = sum+1;
        int cap=1; while(cap<siz_global && cap<MAXN) cap*=2;
        cap=min(cap, MAXN);
        int g=solve_cnt<1>(w);
        int bf=brute(w, cap);
        if(g!=bf){ fail++; if(fail<5) cerr<<"bs "<<g<<" "<<bf<<"\n"; }
    }
    int sf=fail;
    for(int t=0;t<1000;t++){
        int n=4+t%8; vector<int> w(n); int sum=0;
        for(int i=0;i<n;i++){ w[i]=1+rng()%8; sum+=w[i]; }
        siz_global=min(sum+1, MAXN);
        int cap=1; while(cap<siz_global && cap<MAXN) cap*=2; cap=min(cap,MAXN);
        if(solve_cnt<1>(w)!=brute(w,cap)) fail++;
    }
    cout<<"dp_bitset small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}