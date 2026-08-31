#include <bits/stdc++.h>
using namespace std;
const int N = 80;
int val[N], ans[N], c[N][2], f[N], rev[N];
int siz[N], F[N], G[N];
multiset<int> s[N];

namespace LCT{
#define ls(x) c[x][0]
#define rs(x) c[x][1]
void push_up(int x){
	ans[x] = val[x];
	if (ls(x)) ans[x] = max(ans[x], ans[ls(x)]);
	if (rs(x)) ans[x] = max(ans[x], ans[rs(x)]); }
void opt(int x) { if (!x) return; swap(ls(x), rs(x)), rev[x] ^= 1;}
void push_down(int x) {
	if(rev[x]) { if (ls(x)) opt(ls(x)); if (rs(x)) opt(rs(x)); rev[x] = 0; }}
bool nrt(int x) {return x && (ls(f[x]) == x || rs(f[x]) == x);}
bool get(int x) {return x == rs(f[x]);}
void rot(int x){
	int y = f[x], z = f[y], t = get(x), w = c[x][t^1];
	if(nrt(y)) c[z][get(y)] = x;
	c[x][t^1] = y, c[y][t] = w;
	if(w) {f[w] = y;} f[y] = x, f[x] = z; push_up(y); }
void push_all(int x) {if(nrt(x)) push_all(f[x]); push_down(x);}
void splay(int x){
	push_all(x);
	while(nrt(x)){
		int y = f[x];
		if(nrt(y)) rot(get(x) == get(y) ? y : x);
		rot(x);
	} push_up(x); }
void access(int x) {for(int y=0;x;y=x, x=f[x]) splay(x), rs(x) = y, push_up(x);}
void mkrt(int x) {access(x); splay(x); opt(x);}
int findrt(int x){
	access(x), splay(x);
	while(ls(x)) push_down(x), x = ls(x);
	splay(x); return x; }
void split(int x, int y) {mkrt(x), access(y), splay(y);}
bool reach(int x, int y) {mkrt(x); return findrt(y) == x;}
void link(int x, int y) {if(!reach(x, y)) f[x] = y;}
void cut(int x, int y) {if(reach(x, y) && f[y] == x && !ls(y)) f[y] = rs(x) = 0;}
void push_up_diam(int x){
	if (!x) return;
	siz[x] = siz[ls(x)] + siz[rs(x)] + 1;
	F[x] = max(F[ls(x)], F[rs(x)] + siz[ls(x)] + 1);
	G[x] = max(G[rs(x)], G[ls(x)] + siz[rs(x)] + 1);
	if(s[x].size()) 
		F[x] = max(F[x], *(--s[x].end()) + 1 + siz[ls(x)]),
		G[x] = max(G[x], *(--s[x].end()) + 1 + siz[rs(x)]); }
void opt_diam(int x) { if (!x) return; swap(ls(x), rs(x)), swap(F[x], G[x]), rev[x] ^= 1;}
void push_down_diam(int x) {
	if(rev[x]) { if (ls(x)) opt_diam(ls(x)); if (rs(x)) opt_diam(rs(x)); rev[x] = 0; }}
void rot_diam(int x){
	int y = f[x], z = f[y], t = get(x), w = c[x][t^1];
	if(nrt(y)) c[z][get(y)] = x;
	c[x][t^1] = y, c[y][t] = w;
	if(w) {f[w] = y;} f[y] = x, f[x] = z; push_up_diam(y); }
void push_all_diam(int x) {if(nrt(x)) push_all_diam(f[x]); push_down_diam(x);}
void splay_diam(int x){
	push_all_diam(x);
	while(nrt(x)){
		int y = f[x];
		if(nrt(y)) rot_diam(get(x) == get(y) ? y : x);
		rot_diam(x);
	} push_up_diam(x); }
void access_diam(int x) {
	for(int y=0;x;y=x, x=f[x]) {
		splay_diam(x);
		if(rs(x)) s[x].insert(F[rs(x)]);
		int oldF = y ? F[y] : 0;
		if(rs(x) = y) { auto it = s[x].find(oldF); if (it != s[x].end()) s[x].erase(it); }
		push_up_diam(x); } }
void mkrt_diam(int x) {access_diam(x); splay_diam(x); opt_diam(x);}
int findrt_diam(int x){
	access_diam(x), splay_diam(x);
	while(ls(x)) push_down_diam(x), x = ls(x);
	splay_diam(x); return x; }
void split_diam(int x, int y) {mkrt_diam(x), access_diam(y), splay_diam(y);}
bool reach_diam(int x, int y) {mkrt_diam(x); return findrt_diam(y) == x;}
void link_diam(int x, int y) {mkrt_diam(x);if(findrt_diam(y) != x) mkrt_diam(y), f[x] = y, s[y].insert(F[x]), push_up_diam(y);}
void cut_diam(int x, int y) {if(reach_diam(x, y) && f[y] == x && !ls(y)) f[y] = rs(x) = 0, push_up_diam(x);}
#undef ls
#undef rs
}

void reset(int n) {
    for (int i = 0; i <= n; i++) {
        val[i] = i; ans[i] = i; c[i][0]=c[i][1]=0; f[i]=0; rev[i]=0;
        siz[i]=0; F[i]=0; G[i]=0; s[i].clear();
    }
    for (int i = 1; i <= n; i++) LCT::push_up_diam(i);
}

int brute_rt[N];
int findb(int x) { return brute_rt[x]==x ? x : brute_rt[x]=findb(brute_rt[x]); }

mt19937 rng(5);
int fail = 0;
void oops(string msg) { if (fail < 12) cerr << "FAIL " << msg << "\n"; fail++; }

int ecc_nodes(int n, const vector<pair<int,int>>& edges, int src) {
    vector<vector<int>> g(n+1);
    for (auto [u,v]: edges) { g[u].push_back(v); g[v].push_back(u); }
    vector<int> d(n+1, -1);
    queue<int> q; q.push(src); d[src]=0;
    int mx = 0;
    while (!q.empty()) {
        int u=q.front(); q.pop();
        mx = max(mx, d[u]);
        for (int v: g[u]) if (d[v]<0) { d[v]=d[u]+1; q.push(v); }
    }
    return mx + 1; // node count on longest path from src
}

int pathmax(int n, const vector<pair<int,int>>& edges, const vector<int>& w, int a, int b) {
    vector<vector<int>> g(n+1);
    for (auto [u,v]: edges) { g[u].push_back(v); g[v].push_back(u); }
    vector<int> par(n+1,-1), mx(n+1, INT_MIN);
    queue<int> q; q.push(a); par[a]=0; mx[a]=w[a];
    while (!q.empty()) {
        int u=q.front(); q.pop();
        for (int v: g[u]) if (par[v]<0) { par[v]=u; mx[v]=max(mx[u], w[v]); q.push(v); }
    }
    if (par[b]<0) return INT_MIN;
    return mx[b];
}

void test_diam(int n, int nops) {
    reset(n);
    for (int i=1;i<=n;i++) brute_rt[i]=i;
    vector<pair<int,int>> edges;
    auto same = [&](int x,int y){ return findb(x)==findb(y); };
    for (int i=0;i<nops;i++) {
        int op = rng()%5;
        int x = 1+rng()%n, y=1+rng()%n;
        if (op <= 2) { // link
            if (x==y || same(x,y)) continue;
            LCT::link_diam(x,y);
            brute_rt[findb(x)] = findb(y);
            edges.push_back({x,y});
        } else if (op == 3 && !edges.empty()) {
            int id = rng()%edges.size();
            auto [u,v] = edges[id];
            LCT::cut_diam(u,v);
            // rebuild uf
            edges.erase(edges.begin()+id);
            for (int j=1;j<=n;j++) brute_rt[j]=j;
            for (auto [a,b]: edges) if (findb(a)!=findb(b)) brute_rt[findb(a)]=findb(b);
        } else {
            // query connectivity + F
            bool conn = LCT::reach_diam(x,y);
            bool bc = same(x,y);
            if (conn != bc) oops("diam conn");
            if (x==y || bc) {
                LCT::mkrt_diam(x);
                int got = F[x];
                int bf = ecc_nodes(n, edges, x);
                if (got != bf) {
                    oops("F x="+to_string(x)+" got="+to_string(got)+" bf="+to_string(bf)+" n="+to_string(n)+" e="+to_string(edges.size()));
                }
            }
        }
    }
}

void test_path(int n, int nops) {
    // ordinary LCT path max, separate reset without diam s[]
    for (int i=0;i<=n;i++) {
        val[i] = (int)(rng()%100); ans[i]=val[i];
        c[i][0]=c[i][1]=0; f[i]=0; rev[i]=0;
    }
    vector<int> w(n+1); for(int i=1;i<=n;i++) w[i]=val[i];
    for (int i=1;i<=n;i++) brute_rt[i]=i;
    vector<pair<int,int>> edges;
    auto same = [&](int x,int y){ return findb(x)==findb(y); };
    for (int i=0;i<nops;i++) {
        int op = rng()%5;
        int x=1+rng()%n, y=1+rng()%n;
        if (op<=2) {
            if (x==y||same(x,y)) continue;
            LCT::link(x,y);
            brute_rt[findb(x)]=findb(y);
            edges.push_back({x,y});
        } else if (op==3 && !edges.empty()) {
            int id=rng()%edges.size();
            auto [u,v]=edges[id];
            LCT::cut(u,v);
            edges.erase(edges.begin()+id);
            for(int j=1;j<=n;j++) brute_rt[j]=j;
            for (auto [a,b]: edges) if (findb(a)!=findb(b)) brute_rt[findb(a)]=findb(b);
        } else if (x!=y && same(x,y)) {
            LCT::split(x,y);
            int got = ans[y];
            int bf = pathmax(n, edges, w, x, y);
            if (got != bf) oops("pathmax got="+to_string(got)+" bf="+to_string(bf));
        }
    }
}

int main() {
    for (int t=0;t<10000;t++) {
        int n = 3 + t%6;
        test_diam(n, 8+t%10);
        if (fail>15) break;
    }
    int s1=fail;
    for (int t=0;t<10000;t++) {
        int n=3+t%6;
        test_path(n, 8+t%10);
        if (fail>25) break;
    }
    int s2=fail;
    for (int t=0;t<1000;t++) {
        int n=12+t%18;
        test_diam(n, 20+t%25);
        if (fail>40) break;
    }
    int s3=fail;
    for (int t=0;t<1000;t++) {
        int n=12+t%18;
        test_path(n, 20+t%25);
        if (fail>50) break;
    }
    cout << "dp_lct diam_small_fail=" << s1 << " path_small=" << s2-s1
         << " diam_large=" << s3-s2 << " path_large=" << fail-s3
         << " total_fail=" << fail << (fail ? " FAIL\n" : " OK\n");
    return fail ? 1 : 0;
}
