#include <bits/stdc++.h>
using namespace std;
const int N=20;
vector <int> E[N];
vector <int> ml, mr, a, p;
void match (int nl, int nr) {
	ml.assign(nl + 1, 0);
	mr.assign(nr + 1, 0);
	while (true) {
		bool ok = 0;
		a.assign(nl + 1, 0);
		p.assign(nl + 1, 0);
		static queue <int> q;
		while(!q.empty()) q.pop();
		for (int i = 1; i <= nl; i++)
			if (!ml[i]) a[i] = p[i] = i, q.push(i);
		while (!q.empty()) {
			int x = q.front(); q.pop();
			if (ml[a[x]]) continue;
			for (auto y : E[x]) {
				if (!mr[y]) {
					for (ok = 1; y; x = p[x])
						mr[y] = x, swap(ml[x], y);
					break;
				} else if (!p[mr[y]])
					q.push(y = mr[y]), p[y] = x, a[y] = a[x];
			} }
		if (!ok) break; } }
array<vector<int>, 2> min_vertex_cover(int nl, int nr) {
 match(nl, nr); vector <int> l, r;
 for (int i = 1; i <= nl; i++) if (!a[i]) l.push_back(i);
 for (int i = 1; i <= nr; i++) if (a[mr[i]]) r.push_back(i);
 return {l, r}; }
vector<pair<int,int>> min_edge_cover(int nl, int nr) {
 match(nl, nr); vector<pair<int,int>> es;
 vector<int> uL(nl+1), uR(nr+1);
 for (int i = 1; i <= nl; i++) if (ml[i])
  es.push_back({i, ml[i]}), uL[i] = uR[ml[i]] = 1;
 for (int i = 1; i <= nl; i++) if (!uL[i])
  for (int y : E[i]) { es.push_back({i, y}); uL[i] = uR[y] = 1; break; }
 for (int j = 1; j <= nr; j++) if (!uR[j])
  for (int i = 1; i <= nl && !uR[j]; i++)
   for (int y : E[i]) if (y == j) { es.push_back({i, j}); uR[j] = 1; break; }
 return es; }
int brute_match(int nl,int nr){
    vector<int> vis(nr+1), to(nr+1);
    function<int(int)> dfs=[&](int x){
        for(int y:E[x]) if(!vis[y]){ vis[y]=1;
            if(!to[y]||dfs(to[y])){ to[y]=x; return 1; } }
        return 0;
    };
    int m=0;
    for(int i=1;i<=nl;i++){ fill(vis.begin(),vis.end(),0); m+=dfs(i); }
    return m;
}
mt19937 rng(108);
int fail=0;
int main(){
    auto gen=[&](int nl,int nr){
        for(int i=1;i<=nl;i++) E[i].clear();
        int m=rng()%(nl*nr+1);
        set<pair<int,int>> st;
        for(int i=0;i<m;i++){
            int x=1+rng()%nl, y=1+rng()%nr;
            if(st.insert({x,y}).second) E[x].push_back(y);
        }
        return (int)st.size();
    };
    for(int t=0;t<10000;t++){
        int nl=1+t%4, nr=1+t%4;
        gen(nl,nr);
        match(nl,nr);
        int mm=0; for(int i=1;i<=nl;i++) if(ml[i]) mm++;
        int bf=brute_match(nl,nr);
        if(mm!=bf){ fail++; if(fail<6) cerr<<"match "<<mm<<" "<<bf<<"\n"; }
        auto vc=min_vertex_cover(nl,nr);
        // cover all edges, size >= matching
        vector<int> cL(nl+1), cR(nr+1);
        for(int x:vc[0]) cL[x]=1;
        for(int y:vc[1]) cR[y]=1;
        bool cov=1;
        for(int i=1;i<=nl;i++) for(int y:E[i]) if(!cL[i]&&!cR[y]) cov=0;
        if(!cov) { fail++; if(fail<6) cerr<<"uncovered edge\n"; }
        if((int)vc[0].size()+(int)vc[1].size()!=mm) { fail++; if(fail<6) cerr<<"vc size\n"; }
        auto ec=min_edge_cover(nl,nr);
        // |edge cover| = |V'| - matching for vertices with degree>0 typically
        // check: every non-isolated vertex is incident to some cover edge
        vector<int> dL(nl+1), dR(nr+1);
        for(int i=1;i<=nl;i++) dL[i]=E[i].size();
        for(int i=1;i<=nl;i++) for(int y:E[i]) dR[y]++;
        vector<int> eL(nl+1), eR(nr+1);
        for(auto [x,y]:ec) eL[x]++, eR[y]++;
        for(int i=1;i<=nl;i++) if(dL[i] && !eL[i]) { fail++; if(fail<=8) cerr<<"L uncover "<<i<<"\n"; }
        for(int j=1;j<=nr;j++) if(dR[j] && !eR[j]) { fail++; if(fail<=8) cerr<<"R uncover "<<j<<" nr="<<nr<<" es="<<ec.size()<<"\n"; }
    }
    int sf=fail;
    for(int t=0;t<1000;t++){
        int nl=4+t%4, nr=4+t%4;
        gen(nl,nr);
        match(nl,nr);
        int mm=0; for(int i=1;i<=nl;i++) if(ml[i]) mm++;
        if(mm!=brute_match(nl,nr)) fail++;
        auto vc=min_vertex_cover(nl,nr);
        vector<int> cL(nl+1), cR(nr+1);
        for(int x:vc[0]) cL[x]=1; for(int y:vc[1]) cR[y]=1;
        for(int i=1;i<=nl;i++) for(int y:E[i]) if(!cL[i]&&!cR[y]) fail++;
        if((int)vc[0].size()+(int)vc[1].size()!=mm) fail++;
    }
    cout<<"dp_hk small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}