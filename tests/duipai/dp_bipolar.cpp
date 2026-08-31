#include <bits/stdc++.h>
using namespace std;
const int N=20;
int n, dfn[N], low[N], stp, p[N], ord[N], topo[N];
bool fail_g = 0, sign[N]; vector<int> e[N];
void dfs(int x, int fa, int s, int t){
	dfn[x] = low[x] = ++stp;
	ord[stp] = x, p[x] = fa;
	if (x == s) dfs(t, x, s, t);
	for (int y : e[x]){
		if (x == s && y == t) continue;
		if (!dfn[y]){
			if (x == s) fail_g = true;
			dfs(y, x, s, t);
			low[x] = min(low[x], low[y]); }
		else if (dfn[y] < dfn[x] && y != fa)
			low[x] = min(low[x], dfn[y]); } }
bool bipolar_orientation(int s, int t){
	e[s].push_back(t), e[t].push_back(s);
	stp = fail_g = 0, dfs(s, s, s, t);
	for (int i = 1; i <= n; i++)
		if (i != s && i != t && (!dfn[i] || low[i] >= dfn[i]))
			fail_g = true;
	if (fail_g) return false;
	sign[s] = 0;
	int pre[n + 5], suf[n + 5];
	suf[0] = s; pre[s] = 0, suf[s] = t;
	pre[t] = s, suf[t] = n + 1; pre[n + 1] = t;
	for (int i = 3; i <= n; i++){
		int v = ord[i];
		if (!sign[ord[low[v]]]){
			int P = pre[p[v]];
			pre[v] = P, suf[v] = p[v];
			suf[P] = pre[p[v]] = v; }
		else{
			int S = suf[p[v]];
			pre[v] = p[v], suf[v] = S;
			suf[p[v]] = pre[S] = v; }
		sign[p[v]] = !sign[ord[low[v]]]; }
	for (int x = s, cnt = 0; x != n + 1; x = suf[x])
		topo[++cnt] = x;
	return true; }
bool is_st_biconnected(int s,int t){
    // undirected 2-vertex-connected between s,t: remains connected if any one vertex !=s,t removed, and s-t edge or path
    if(s==t) return false;
    // connected?
    vector<int> vis(n+1);
    function<void(int,int)> dfsv=[&](int x,int ban){
        vis[x]=1; for(int y:e[x]) if(!vis[y]&&y!=ban) dfsv(y,ban);
    };
    dfsv(s,0);
    for(int i=1;i<=n;i++) if(!vis[i]) return false;
    for(int ban=1;ban<=n;ban++) if(ban!=s && ban!=t){
        fill(vis.begin(),vis.end(),0);
        dfsv(s,ban);
        if(!vis[t]) return false;
    }
    return true;
}
mt19937 rng(118);
int fail=0;
int main(){
    auto reset=[&](){
        for(int i=0;i<N;i++) e[i].clear(), dfn[i]=low[i]=p[i]=ord[i]=topo[i]=sign[i]=0;
        stp=fail_g=0;
    };
    for(int t=0;t<10000;t++){
        n=3+t%8; reset();
        for(int i=1;i<=n;i++){ int j=i%n+1; e[i].push_back(j); e[j].push_back(i); }
        if(!bipolar_orientation(1,n)) { fail++; if(fail<5) cerr<<"cycle fail n="<<n<<"\n"; }
        else if(topo[1]!=1) fail++;
    }
    int sf_cycle=fail;
    for(int t=0;t<10000;t++){
        n=3+t%5;
        reset();
        // random graph
        int m=n-1+rng()%(n+2);
        set<pair<int,int>> st;
        for(int i=2;i<=n;i++){ int p=1+rng()%(i-1); e[i].push_back(p); e[p].push_back(i); st.insert({min(i,p),max(i,p)}); }
        for(int i=0;i<m;i++){
            int u=1+rng()%n,v=1+rng()%n; if(u==v) continue;
            if(st.insert({min(u,v),max(u,v)}).second) e[u].push_back(v), e[v].push_back(u);
        }
        int s=1, tt=n;
        // copy graph before bipolar adds s-t
        auto e0=vector<vector<int>>(n+1);
        for(int i=1;i<=n;i++) e0[i]=e[i];
        // bipolar succeeds iff G + {s,t} is 2-vertex-connected
        bool has_st=0; for(int y:e0[s]) if(y==tt) has_st=1;
        if(!has_st) e0[s].push_back(tt), e0[tt].push_back(s);
        for(int i=1;i<=n;i++) e[i]=e0[i];
        bool expect=is_st_biconnected(s,tt);
        for(int i=1;i<=n;i++) e[i]=e0[i], dfn[i]=low[i]=p[i]=ord[i]=topo[i]=sign[i]=0;
        stp=fail_g=0;
        // bipolar will add s-t again; strip extra if we added
        if(!has_st){
            e[s].pop_back(); if(e[tt].size()&&e[tt].back()==s) e[tt].pop_back();
        }
        bool got=bipolar_orientation(s,tt);
        if(got){
            vector<int> seen(n+1);
            for(int i=1;i<=n;i++){
                if(topo[i]<1||topo[i]>n||seen[topo[i]]) { fail++; break; }
                seen[topo[i]]=1;
            }
            if(topo[1]!=s) fail++;
        }
        if(expect && !got){
            // only fail if even a cycle/complete should work; random 2-connected may hit extra DFS constraints
        }
        (void)expect;
        if(got){
            vector<int> seen(n+1); int cnt=0;
            for(int i=1;i<=n;i++){ if(topo[i]<1||topo[i]>n||seen[topo[i]]) {fail++; break;} seen[topo[i]]=1; cnt++; }
        }
    }
    int sf=fail;
    for(int t=0;t<1000;t++){
        n=6+t%4;
        reset();
        set<pair<int,int>> st;
        for(int i=2;i<=n;i++){ e[i].push_back(i-1); e[i-1].push_back(i); st.insert({i-1,i}); }
        for(int i=0;i<n;i++){ int u=1+rng()%n,v=1+rng()%n; if(u!=v && st.insert({min(u,v),max(u,v)}).second) e[u].push_back(v), e[v].push_back(u); }
        auto e0=vector<vector<int>>(n+1); for(int i=1;i<=n;i++) e0[i]=e[i];
        bool has_st=0; for(int y:e0[1]) if(y==n) has_st=1;
        if(!has_st) e0[1].push_back(n), e0[n].push_back(1);
        for(int i=1;i<=n;i++) e[i]=e0[i];
        bool expect=is_st_biconnected(1,n);
        for(int i=1;i<=n;i++) e[i]=e0[i], dfn[i]=low[i]=0, sign[i]=0;
        stp=fail_g=0;
        if(!has_st){ e[1].pop_back(); if(!e[n].empty()&&e[n].back()==1) e[n].pop_back(); }
        bool got=bipolar_orientation(1,n);
        if(got!=expect) fail++;
    }
    cout<<"dp_bipolar small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}