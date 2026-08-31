#include <bits/stdc++.h>
using namespace std;
int SubsetSum(vector <int> &a, int t) {
  if (a.empty() || t < 0) return 0;
  int B = *max_element(a.begin(), a.end());
  int n = (int) a.size(), s = 0, i = 0;
  while (i < n && s + a[i] <= t) s += a[i ++];
  if (i == n) return s;
  vector <int> f(2 * B + 1, -1), pre (B + 1, -1);
  f[s - (t - B)] = i;
  for ( ; i < n; i++) {
	s += a[i];
	for (int d = 0; d <= B; d++) pre[d] = max(0, f[d + B]);
	for (int d = B; d >= 0; d--)
	  f[d + a[i]] = max(f[d + a[i]], f[d]);
	for (int d = 2 * B; d > B; --d)
	  for (int j = pre[d - B]; j < f[d]; j++)
		f[d - a[j]] = max(f[d - a[j]], j); }
  for (i = 0; i <= B; i++) if (f[B - i] >= 0) return t - i;
  return 0;}
int brute(const vector<int>& a, int t){
    int n=a.size(), best=0;
    for(int m=0;m<(1<<n);m++){
        int s=0; for(int i=0;i<n;i++) if(m>>i&1) s+=a[i];
        if(s<=t) best=max(best,s);
    }
    return best;
}
mt19937 rng(17);
int fail=0;
int main(){
    for(int t=0;t<10000;t++){
        int n=1+t%8;
        vector<int> a(n);
        for(int i=0;i<n;i++) a[i]=1+rng()%8;
        int T=rng()%40;
        int got=SubsetSum(a,T), bf=brute(a,T);
        if(got!=bf){ fail++; if(fail<8) cerr<<"got="<<got<<" bf="<<bf<<" T="<<T<<"\n"; }
    }
    int sf=fail;
    for(int t=0;t<1000;t++){
        int n=8+t%4; // 8..11, 2^11=2048
        vector<int> a(n);
        for(int i=0;i<n;i++) a[i]=1+rng()%15;
        int T=rng()%80;
        int got=SubsetSum(a,T), bf=brute(a,T);
        if(got!=bf) fail++;
    }
    cout<<"dp_subset small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}
