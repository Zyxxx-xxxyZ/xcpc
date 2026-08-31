#include <bits/stdc++.h>
using namespace std;
mt19937 rng(18);
int fail=0;
long long brute_c4(int n, const vector<pair<int,int>>& ed){
    vector<vector<int>> g(n+1);
    for(auto [u,v]: ed){ g[u].push_back(v); g[v].push_back(u); }
    long long ans=0;
    // count undirected 4-cycles / 8? typically each cycle counted 4*2=8 times or 4?
    // snippet: for each i, walk i->x(dir)->y(undir), y>i by deg. ans += cnt[y]++
    // this counts pairs of common neighbors, each C4 counted twice per pair of opposite vertices...
    // We'll compare against same directed-degree method brute: enumerate all 4-cycles uniquely then *2?
    // Safer: implement the SAME walk as brute with adjacency matrix common-neighbor.
    vector<vector<int>> adj(n+1, vector<int>(n+1));
    for(auto [u,v]: ed) adj[u][v]=adj[v][u]=1;
    long long c=0;
    for(int a=1;a<=n;a++) for(int b=a+1;b<=n;b++){
        int cm=0;
        for(int k=1;k<=n;k++) if(k!=a&&k!=b && adj[a][k]&&adj[b][k]) cm++;
        c += 1LL*cm*(cm-1)/2;
    }
    // each C4 counted twice (two pairs of opposite vertices)
    return c;
}
long long snippet(int n, vector<int> x, vector<int> y, int m){
    vector<int> deg(n+1);
    vector<vector<int>> g(n+1), e(n+1);
    for(int i=1;i<=m;i++){ deg[x[i]]++; deg[y[i]]++; }
    long long ans=0;
    vector<int> cnt(n+1);
    for(int i=1;i<=m;i++) {
        g[x[i]].push_back(y[i]); g[y[i]].push_back(x[i]);
        if(deg[x[i]] > deg[y[i]] || (deg[x[i]] == deg[y[i]] && x[i] > y[i])) swap(x[i], y[i]);
        e[x[i]].push_back(y[i]);}
    for(int i=1;i<=n;i++) {
        for(auto xx : e[i]) for(auto yy : g[xx])
            if(yy != i && (deg[i] < deg[yy] || (deg[i] == deg[yy] && i < yy)))
                ans += cnt[yy]++;
        for(auto xx : e[i]) for(auto yy : g[xx]) cnt[yy] = 0;}
    return ans;
}
int main(){
    for(int t=0;t<10000;t++){
        int n=3+t%6, m=t%12;
        set<pair<int,int>> st;
        vector<int> x(m+1), y(m+1);
        vector<pair<int,int>> ed;
        int em=0;
        for(int i=0;i<40 && em<m;i++){
            int a=1+rng()%n, b=1+rng()%n; if(a==b) continue;
            if(a>b) swap(a,b);
            if(st.count({a,b})) continue;
            st.insert({a,b}); em++;
            x[em]=a; y[em]=b; ed.push_back({a,b});
        }
        m=em; x.resize(m+1); y.resize(m+1);
        long long got=snippet(n,x,y,m);
        long long bf=brute_c4(n,ed);
        // brute_c4 对每个四元环的两对对角点各计一次, 故 /2 才是环数
        if(bf % 2){ fail++; if(fail<5) cerr<<"odd brute\n"; }
        if(got != bf/2){ fail++; if(fail<8) cerr<<"got="<<got<<" bf/2="<<bf/2<<" n="<<n<<" m="<<m<<"\n"; }
    }
    int sf=fail;
    for(int t=0;t<1000;t++){
        int n=8+t%10, m=t%25;
        set<pair<int,int>> st;
        vector<int> x,y; x.push_back(0); y.push_back(0);
        vector<pair<int,int>> ed;
        for(int i=0;i<80 && (int)ed.size()<m;i++){
            int a=1+rng()%n, b=1+rng()%n; if(a==b) continue;
            if(a>b) swap(a,b);
            if(st.count({a,b})) continue;
            st.insert({a,b});
            x.push_back(a); y.push_back(b); ed.push_back({a,b});
        }
        m=ed.size();
        long long got=snippet(n,x,y,m);
        long long bf=brute_c4(n,ed);
        if(got != bf/2) fail++;
    }
    cout<<"dp_c4 small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}
