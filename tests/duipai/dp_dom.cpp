#include <bits/stdc++.h>
using namespace std;
const int MAXN=30;
vector<int> G[MAXN], R[MAXN], son[MAXN];
int ufs[MAXN];
int idom[MAXN], sdom[MAXN], anc[MAXN];
int pr[MAXN], dfn[MAXN], id[MAXN], stamp;
int findufs(int x) { if (ufs[x] == x) return x;
	int t = ufs[x]; ufs[x] = findufs(ufs[x]);
	if (dfn[sdom[anc[x]]] > dfn[sdom[anc[t]]])
		anc[x] = anc[t];
	return ufs[x]; }
void dfs(int x) {
	dfn[x] = ++stamp; id[stamp] = x; sdom[x] = x;
	for (int y : G[x]) if (!dfn[y]) { pr[y] = x; dfs(y); } }
void get_dominator(int n) {
	for (int i = 1; i <= n; i++) ufs[i] = anc[i] = i;
	dfs(1);
	for (int i = stamp; i > 1; i--) { int x = id[i];
		for (int y : R[x]) if (dfn[y]) { findufs(y);
			if (dfn[sdom[x]] > dfn[sdom[anc[y]]])
				sdom[x] = sdom[anc[y]]; }
		son[sdom[x]].push_back(x); ufs[x] = pr[x];
		for (int u : son[pr[x]]) { findufs(u);
			idom[u] = (sdom[u] == sdom[anc[u]] ?
				pr[x] : anc[u]); }
		son[pr[x]].clear(); }
	for (int i = 2; i <= stamp; i++) { int x = id[i];
		if (idom[x] != sdom[x]) idom[x] = idom[idom[x]];
		son[idom[x]].push_back(x); } }
int brute_idom(int n, int v){
    auto dominates=[&](int u){
        if(u==v) return false;
        vector<int> vis(n+1);
        queue<int> q;
        if(u!=1){ q.push(1); vis[1]=1; }
        while(!q.empty()){
            int x=q.front(); q.pop();
            for(int y:G[x]) if(!vis[y] && y!=u){ vis[y]=1; q.push(y); }
        }
        return !vis[v];
    };
    vector<int> dep(n+1,-1); queue<int> q; q.push(1); dep[1]=0;
    while(!q.empty()){ int x=q.front(); q.pop(); for(int y:G[x]) if(dep[y]<0){ dep[y]=dep[x]+1; q.push(y);} }
    int idm=0, best=-1;
    for(int u=1;u<=n;u++) if(dominates(u) && dep[u]>best){ best=dep[u]; idm=u; }
    return idm;
}
mt19937 rng(117);
int fail=0;
int main(){
    auto reset=[&](int n){
        stamp=0;
        for(int i=0;i<=n;i++){
            G[i].clear(); R[i].clear(); son[i].clear();
            dfn[i]=id[i]=idom[i]=sdom[i]=anc[i]=pr[i]=ufs[i]=0;
        }
    };
    for(int t=0;t<10000;t++){
        int n=3+t%4;
        reset(n);
        // random DAG-ish from 1: edges i->j j>i plus some
        for(int i=1;i<n;i++){
            int j=i+1+rng()%(n-i);
            G[i].push_back(j); R[j].push_back(i);
        }
        int extra=rng()%(n);
        for(int k=0;k<extra;k++){
            int u=1+rng()%n, v=1+rng()%n; if(u==v) continue;
            G[u].push_back(v); R[v].push_back(u);
        }
        get_dominator(n);
        for(int v=2;v<=n;v++) if(dfn[v]){
            int bf=brute_idom(n,v);
            if(idom[v]!=bf){ fail++; if(fail<6) cerr<<"n="<<n<<" v="<<v<<" idom="<<idom[v]<<" bf="<<bf<<"\n"; }
        }
        if(fail>12) break;
    }
    int sf=fail;
    for(int t=0;t<1000;t++){
        int n=6+t%4;
        reset(n);
        for(int i=1;i<n;i++){ G[i].push_back(i+1); R[i+1].push_back(i); }
        for(int k=0;k<n;k++){ int u=1+rng()%n,v=1+rng()%n; if(u!=v){ G[u].push_back(v); R[v].push_back(u);} }
        get_dominator(n);
        for(int v=2;v<=n;v++) if(dfn[v]){
            if(idom[v]!=brute_idom(n,v)) fail++;
        }
        if(fail>20) break;
    }
    cout<<"dp_dom small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}