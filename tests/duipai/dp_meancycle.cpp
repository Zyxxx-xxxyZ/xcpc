#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll Inf = (ll)4e18;
const int N = 20, M = 80;
mt19937 rng(20);
int fail=0;
double snippet(int n, int m, int* u, int* v, int* w){
    ll f[N][N];
    for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) f[i][j] = Inf;
    for (int i = 1; i <= n; i++) f[0][i] = 0;
    for(int i = 1; i <= n; i ++)
        for(int j = 0; j < m; j ++)
            f[i][v[j]] = min(f[i][v[j]], f[i - 1][u[j]] + w[j]);
    double ans = Inf;
    for(int i = 1; i <= n; i ++) if (f[n][i] < Inf / 2) {
        double t = -Inf; bool ok = 0;
        for(int j = 0; j < n; j ++) if (f[j][i] < Inf / 2) {
            t = max(t, (f[n][i] - f[j][i]) / (double)(n - j)); ok = 1; }
        if (ok) ans = min(t, ans); }
    return ans;
}
double brute_min_mean(int n, int m, int* u, int* v, int* w){
    // enumerate simple cycles via DFS
    vector<vector<pair<int,int>>> g(n+1);
    for(int i=0;i<m;i++) g[u[i]].push_back({v[i], w[i]});
    double best = 1e100;
    bool found=false;
    vector<int> on(n+1), st, ws;
    function<void(int,int)> dfs=[&](int x, int start){
        on[x]=1; st.push_back(x);
        for(auto [y,ww]: g[x]){
            ws.push_back(ww);
            if(y==start && st.size()>=1){
                int s=0; for(int t:ws) s+=t;
                best=min(best, s/(double)ws.size()); found=true;
            }
            if(!on[y] && (int)st.size()<n) dfs(y, start);
            ws.pop_back();
        }
        st.pop_back(); on[x]=0;
    };
    for(int s=1;s<=n;s++) dfs(s,s);
    return found?best:Inf;
}
int main(){
    int u[M],v[M],w[M];
    for(int t=0;t<10000;t++){
        int n=2+t%4, m=1+t%8;
        for(int i=0;i<m;i++){
            u[i]=1+rng()%n; v[i]=1+rng()%n; w[i]=(int)(rng()%7)-2;
        }
        double got=snippet(n,m,u,v,w);
        double bf=brute_min_mean(n,m,u,v,w);
        bool ginf=got>1e17, binf=bf>1e17;
        if(ginf||binf){
            if(ginf!=binf){ fail++; if(fail<8) cerr<<"inf mismatch\n"; }
        } else if(fabs(got-bf)>1e-6){
            fail++; if(fail<8) cerr<<"got="<<got<<" bf="<<bf<<"\n";
        }
    }
    int sf=fail;
    for(int t=0;t<1000;t++){
        int n=4+t%3, m=3+t%12;
        for(int i=0;i<m;i++){
            u[i]=1+rng()%n; v[i]=1+rng()%n; w[i]=(int)(rng()%11)-5;
        }
        double got=snippet(n,m,u,v,w);
        double bf=brute_min_mean(n,m,u,v,w);
        bool ginf=got>1e17, binf=bf>1e17;
        if(ginf||binf){ if(ginf!=binf) fail++; }
        else if(fabs(got-bf)>1e-5) fail++;
    }
    cout<<"dp_meancycle small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}
