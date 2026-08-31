#include <bits/stdc++.h>
using namespace std;
const int N = 40;
int G[N][N], n;
int dis[N], vis[N], bin[N];
int contract(int &s, int &t) {
	memset(vis, 0, sizeof(vis));
	memset(dis, 0, sizeof(dis));
	int i, j, k, mincut, maxc;
	for (i = 1; i <= n; i++) {
		k = -1; maxc = -1;
		for (j = 1; j <= n; j++)
			if (!bin[j] && !vis[j] && dis[j] > maxc) {
				k = j;
				maxc = dis[j]; }
		if (k == -1) return mincut;
		s = t; t = k; mincut = maxc; vis[k] = true;
		for (j = 1; j <= n; j++)
			if (!bin[j] && !vis[j]) dis[j] += G[k][j];
	} return mincut; }
const int inf = 0x3f3f3f3f;
int solve() {
	int mincut, i, j, s, t, ans;
	if (n <= 1) return 0;
	memset(bin, 0, sizeof bin);
	for (mincut = inf, i = 1; i < n; i++) {
		ans = contract(s, t);
		bin[t] = true;
		if (mincut > ans) mincut = ans;
		if (mincut == 0) return 0;
		for (j = 1; j <= n; j++)
			if (!bin[j]) G[s][j] = (G[j][s] += G[j][t]);
	} return mincut; }
int brute_mincut(int n_, const vector<vector<int>>& w){
    if(n_<=1) return 0;
    int best=INT_MAX;
    int Nmask=1<<n_;
    for(int mask=1; mask<Nmask-1; mask++){
        if(!(mask&1)) continue; // s=0 in set, t not: fix node 0 in S to avoid double
        // actually global min cut: all partitions
    }
    best=INT_MAX;
    for(int mask=1; mask<Nmask-1; mask++){
        int cut=0;
        for(int i=0;i<n_;i++) if(mask>>i&1)
            for(int j=0;j<n_;j++) if(!(mask>>j&1))
                cut += w[i+1][j+1];
        best=min(best, cut);
    }
    return best;
}
mt19937 rng(19);
int fail=0;
int main(){
    for(int t=0;t<10000;t++){
        n=1+t%6;
        memset(G,0,sizeof G);
        vector<vector<int>> w(n+1, vector<int>(n+1));
        int m=t%10;
        for(int i=0;i<m;i++){
            int a=1+rng()%n, b=1+rng()%n, ww=1+rng()%5;
            if(a==b) continue;
            G[a][b]+=ww; G[b][a]+=ww;
            w[a][b]+=ww; w[b][a]+=ww;
        }
        vector<vector<int>> G0(n+1, vector<int>(n+1));
        for(int i=1;i<=n;i++) for(int j=1;j<=n;j++) G0[i][j]=G[i][j];
        int got=solve();
        int bf=brute_mincut(n,w);
        if(got!=bf){ fail++; if(fail<8) cerr<<"got="<<got<<" bf="<<bf<<" n="<<n<<"\n"; }
    }
    int sf=fail;
    for(int t=0;t<1000;t++){
        n=6+t%4; // 6-9, 2^9=512
        memset(G,0,sizeof G);
        vector<vector<int>> w(n+1, vector<int>(n+1));
        int m=n+rng()%(n*2);
        for(int i=0;i<m;i++){
            int a=1+rng()%n, b=1+rng()%n, ww=1+rng()%7;
            if(a==b) continue;
            G[a][b]+=ww; G[b][a]+=ww;
            w[a][b]+=ww; w[b][a]+=ww;
        }
        int got=solve();
        int bf=brute_mincut(n,w);
        if(got!=bf) fail++;
    }
    cout<<"dp_sw small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}
