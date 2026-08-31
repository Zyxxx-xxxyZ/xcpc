#include <bits/stdc++.h>
using namespace std;
const int MAXN = 200;
int n;
namespace Leftist{
	int a[MAXN], ls[MAXN], rs[MAXN], dis[MAXN], fa[MAXN];
	bool vis[MAXN];
	void init() {dis[0] = -1; for(int i=1;i<=n;i++) fa[i] = i;}
	int find(int x) {return fa[x] == x ? x : fa[x] = find(fa[x]);}
	int _merge(int x, int y){
		if(!x || !y) return x + y;
		if(a[y] < a[x]) swap(x, y);
		rs[x] = _merge(rs[x], y);
		if(dis[ls[x]] < dis[rs[x]]) swap(ls[x], rs[x]);
		dis[x] = dis[rs[x]] + 1;
		return x;
	}
	void merge(int x, int y){
		if(vis[x] ||vis[y]) return;
		x = find(x), y = find(y);
		if(x != y) fa[x] = fa[y] = _merge(x, y);
	}
	int del(int x){
		if(vis[x]) return -1;
		x = find(x); vis[x] = true;
		int ret = a[x];
		fa[ls[x]] = fa[rs[x]] = fa[x] = _merge(ls[x], rs[x]);
		ls[x] = rs[x] = dis[x] = 0;
		return ret;
	}
}
mt19937 rng(104);
int fail=0;
void oops(string s){ if(fail<8) cerr<<"FAIL "<<s<<"\n"; fail++; }
void reset(int nn){
    n=nn;
    memset(Leftist::ls,0,sizeof Leftist::ls);
    memset(Leftist::rs,0,sizeof Leftist::rs);
    memset(Leftist::dis,0,sizeof Leftist::dis);
    memset(Leftist::vis,0,sizeof Leftist::vis);
    memset(Leftist::fa,0,sizeof Leftist::fa);
    Leftist::init();
}
int main(){
    // merge-all then extract-min n times via node 1 (find walks to current root)
    for(int t=0;t<10000;t++){
        int nn=2+t%8;
        for(int i=1;i<=nn;i++) Leftist::a[i]=1+rng()%50;
        reset(nn);
        vector<int> vals(Leftist::a+1, Leftist::a+nn+1);
        for(int i=2;i<=nn;i++) Leftist::merge(1,i);
        vector<int> got;
        for(int r=0;r<nn;r++){
            int v=-1;
            for(int i=1;i<=nn;i++){ v=Leftist::del(i); if(v!=-1) break; }
            got.push_back(v);
        }
        sort(vals.begin(), vals.end());
        if(got!=vals){ fail++; if(fail<6){
            cerr<<"n="<<nn<<" a="; for(int i=1;i<=nn;i++) cerr<<Leftist::a[i]<<" ";
            cerr<<" got="; for(int x:got) cerr<<x<<" ";
            cerr<<" vals="; for(int x:vals) cerr<<x<<" "; cerr<<"\n";
        } }
        int extra=-1; for(int i=1;i<=nn;i++){ extra=Leftist::del(i); if(extra!=-1) break; }
        if(extra!=-1) fail++;
    }
    int sf=fail;
    for(int t=0;t<1000;t++){
        int nn=8+t%20;
        for(int i=1;i<=nn;i++) Leftist::a[i]=(int)rng()%1000;
        reset(nn);
        vector<int> vals(Leftist::a+1, Leftist::a+nn+1);
        // random merges then extract remaining via a live node
        for(int i=0;i<nn*2;i++) Leftist::merge(1+rng()%nn, 1+rng()%nn);
        // ensure connected
        for(int i=2;i<=nn;i++) Leftist::merge(1,i);
        vector<int> got;
        for(int r=0;r<nn;r++){
            int v=-1;
            for(int i=1;i<=nn;i++){ v=Leftist::del(i); if(v!=-1) break; }
            got.push_back(v);
        }
        sort(vals.begin(), vals.end());
        sort(got.begin(), got.end());
        if(got!=vals) fail++;
    }
    cout<<"dp_leftist small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}
