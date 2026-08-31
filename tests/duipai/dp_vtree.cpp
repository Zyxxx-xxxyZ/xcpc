#include <bits/stdc++.h>
using namespace std;
const int N=80;
vector<int> e[N];
int id[N], dfn, dep[N], fa[N][8], n;
void dfs(int x,int p){
    id[x]=++dfn; dep[x]=dep[p]+1; fa[x][0]=p;
    for(int k=1;k<8;k++) fa[x][k]=fa[fa[x][k-1]][k-1];
    for(int y:e[x]) if(y!=p) dfs(y,x);
}
int lca(int x,int y){
    if(dep[x]<dep[y]) swap(x,y);
    for(int k=7;k>=0;k--) if(dep[fa[x][k]]>=dep[y]) x=fa[x][k];
    if(x==y) return x;
    for(int k=7;k>=0;k--) if(fa[x][k]!=fa[y][k]) x=fa[x][k], y=fa[y][k];
    return fa[x][0];
}
int s[N];
namespace vt { vector<int> e[N]; void push(int u,int v){ e[u].push_back(v); e[v].push_back(u); } }
bool cmp(int x, int y) {return id[x] < id[y];}
void build(vector<int> v){
	sort(v.begin(), v.end(), cmp);
	v.erase(unique(v.begin(), v.end()), v.end());
	int top = 0;
	s[top=1] = 1; vt::e[1].clear();
	for(auto x : v){
		if(x == 1) continue;
		int l = lca(x, s[top]);
		if(l != s[top]){
			while(id[l] < id[s[top-1]]) vt::push(s[top-1], s[top]), top--;
			if(l != s[top-1]) vt::e[l].clear(), vt::push(l, s[top]), s[top] = l;
			else vt::push(l, s[top--]);
		}
		s[++top] = x; vt::e[x].clear();
	}
	for(int i=1;i<top;i++) vt::push(s[i], s[i+1]);
}
mt19937 rng(116);
int fail=0;
int main(){
    auto tree=[&](int nn){
        n=nn; dfn=0;
        for(int i=1;i<=n;i++) e[i].clear(), vt::e[i].clear();
        for(int i=2;i<=n;i++){ int p=1+rng()%(i-1); e[i].push_back(p); e[p].push_back(i); }
        dfs(1,0);
    };
    auto orig_lca=[&](int x,int y){ return lca(x,y); };
    for(int t=0;t<10000;t++){
        int nn=4+t%8;
        tree(nn);
        int k=1+t%(nn);
        vector<int> key(k);
        for(int i=0;i<k;i++) key[i]=1+rng()%nn;
        key.push_back(1);
        for(int i=1;i<=nn;i++) vt::e[i].clear();
        build(key);
        // virtual tree should contain all keys and their LCAs; pairwise LCA in vt == original among nodes in vt
        set<int> nodes; nodes.insert(1);
        function<void(int,int)> walk=[&](int x,int p){
            nodes.insert(x);
            for(int y:vt::e[x]) if(y!=p) walk(y,x);
        };
        walk(1,0);
        for(int x:key) if(!nodes.count(x) && x){ fail++; break; }
        // LCA of two keys in original equals LCA in virtual tree if we walk
        vector<int> nd(nodes.begin(), nodes.end());
        auto vt_lca=[&](int x,int y){
            // BFS parent on virtual tree
            vector<int> p(n+1,-1), d(n+1);
            queue<int> q; q.push(1); p[1]=0;
            while(!q.empty()){
                int u=q.front(); q.pop();
                for(int v:vt::e[u]) if(p[v]<0){ p[v]=u; d[v]=d[u]+1; q.push(v); }
            }
            if(p[x]<0||p[y]<0) return -1;
            while(d[x]>d[y]) x=p[x];
            while(d[y]>d[x]) y=p[y];
            while(x!=y) x=p[x], y=p[y];
            return x;
        };
        for(int i=0;i<(int)key.size();i++) for(int j=i+1;j<(int)key.size();j++){
            int u=key[i], v=key[j];
            int o=orig_lca(u,v);
            int w=vt_lca(u,v);
            if(w!=o){ fail++; if(fail<5) cerr<<"lca "<<u<<" "<<v<<" orig="<<o<<" vt="<<w<<"\n"; goto nx; }
        }
        nx:;
    }
    int sf=fail;
    for(int t=0;t<1000;t++){
        int nn=12+t%20;
        tree(nn);
        int k=3+t%10;
        vector<int> key(k);
        for(int i=0;i<k;i++) key[i]=1+rng()%nn;
        for(int i=1;i<=nn;i++) vt::e[i].clear();
        build(key);
        set<int> nodes; function<void(int,int)> walk=[&](int x,int p){ nodes.insert(x); for(int y:vt::e[x]) if(y!=p) walk(y,x); };
        walk(1,0);
        for(int x:key) if(!nodes.count(x)) fail++;
    }
    cout<<"dp_vtree small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}