#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;
#define bit(i) (1ULL << (i))
ll brute_maxclique_count(int n, ull* E){
    ll ans=0;
    for(int m=0;m<(1<<n);m++){
        bool ok=1;
        for(int i=0;i<n && ok;i++) if(m>>i&1)
            for(int j=i+1;j<n;j++) if(m>>j&1)
                if(!(E[i]>>j&1)) { ok=0; break; }
        if(ok) ans++; // all cliques including empty? bron-kerbosch maximal
    }
    return ans;
}
// The snippet is MAXIMAL clique count (Bron-Kerbosch), not all cliques
ll snippet_maximal(int n, ull* E){
    ll ans=0;
    vector<ull> sol;
    function<void(ull,ull,ull)> dfs=[&](ull P, ull X, ull R){
        if (!P && !X) { ++ans; sol.push_back(R); return; }
        ull Q = P & ~E[__builtin_ctzll(P | X)];
        for (int i; Q; Q &= ~bit(i)) {
            i = __builtin_ctzll(Q);
            dfs(P & E[i], X & E[i], R | bit(i));
            P &= ~bit(i), X |= bit(i); }
    };
    ans=0; dfs(n==64?~0ULL:bit(n)-1, 0, 0);
    return ans;
}
ll brute_maximal(int n, ull* E){
    ll ans=0;
    for(int m=1;m<(1<<n);m++){
        bool clique=1;
        for(int i=0;i<n && clique;i++) if(m>>i&1)
            for(int j=i+1;j<n;j++) if(m>>j&1)
                if(!(E[i]>>j&1)) clique=0;
        if(!clique) continue;
        bool maxi=1;
        for(int v=0;v<n;v++) if(!(m>>v&1)){
            bool can=1;
            for(int i=0;i<n;i++) if(m>>i&1) if(!(E[v]>>i&1)) can=0;
            if(can){ maxi=0; break; }
        }
        if(maxi) ans++;
    }
    return ans;
}
mt19937 rng(23);
int fail=0;
int main(){
    ull E[64];
    for(int t=0;t<10000;t++){
        int n=1+t%6;
        memset(E,0,sizeof E);
        for(int i=0;i<n;i++) for(int j=i+1;j<n;j++) if(rng()%2){
            E[i]|=bit(j); E[j]|=bit(i);
        }
        ll got=snippet_maximal(n,E);
        ll bf=brute_maximal(n,E);
        if(got!=bf){ fail++; if(fail<8) cerr<<"got="<<got<<" bf="<<bf<<" n="<<n<<"\n"; }
    }
    int sf=fail;
    for(int t=0;t<1000;t++){
        int n=7+t%5; // 7-11
        memset(E,0,sizeof E);
        for(int i=0;i<n;i++) for(int j=i+1;j<n;j++) if(rng()%3){
            E[i]|=bit(j); E[j]|=bit(i);
        }
        ll got=snippet_maximal(n,E);
        ll bf=brute_maximal(n,E);
        if(got!=bf) fail++;
    }
    cout<<"dp_clique small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}
