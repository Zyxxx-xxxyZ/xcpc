#include <bits/stdc++.h>
using namespace std;
const int N=2000;
struct AC{
int ch[N][26], fail[N], tot;
vector<int> tag[N];
void ins(string s, int _idx){
	int x = 0;
	for(auto c : s){
		int v = c - 'a';
		if(!ch[x][v]) ch[x][v] = ++tot;
		x = ch[x][v]; } tag[x].push_back(_idx); }
void build(){
	queue<int> q;
	for(int i=0;i<26;i++) if(ch[0][i]) q.push(ch[0][i]);
	while(!q.empty()){
		int x = q.front(); q.pop();
		for(int i=0;i<26;i++) {
			if(ch[x][i]) fail[ch[x][i]] = ch[fail[x]][i], q.push(ch[x][i]);
			else ch[x][i] = ch[fail[x]][i]; } } }
} ac;
mt19937 rng(109);
int fail=0;
int main(){
    auto run=[&](int npat, int plen, int tlen){
        memset(ac.ch,0,sizeof ac.ch); memset(ac.fail,0,sizeof ac.fail);
        for(int i=0;i<N;i++) ac.tag[i].clear(); ac.tot=0;
        vector<string> pat(npat);
        for(int i=0;i<npat;i++){
            int L=1+rng()%plen;
            string s; for(int j=0;j<L;j++) s+='a'+rng()%3;
            pat[i]=s; ac.ins(s,i);
        }
        ac.build();
        string t; int n=1+rng()%tlen; for(int i=0;i<n;i++) t+='a'+rng()%3;
        vector<int> cnt(npat), bf(npat);
        int x=0;
        for(char c: t){
            x=ac.ch[x][c-'a'];
            for(int y=x;y;y=ac.fail[y]) for(int id: ac.tag[y]) cnt[id]++;
            // also node 0 tags
            for(int id: ac.tag[0]) cnt[id]++;
        }
        // brute
        for(int i=0;i<npat;i++){
            for(size_t p=0;p<t.size();){
                auto f=t.find(pat[i], p);
                if(f==string::npos) break;
                bf[i]++; p=f+1;
            }
        }
        // fail links may miss tag[0] logic; count occurrences of each pattern
        // recompute cnt properly: at each position, follow fail and collect
        fill(cnt.begin(),cnt.end(),0);
        x=0;
        for(char c: t){
            x=ac.ch[x][c-'a'];
            for(int y=x; y; y=ac.fail[y]) for(int id:ac.tag[y]) cnt[id]++;
        }
        if(cnt!=bf){ fail++; if(fail<6){
            cerr<<"pat mismatch\n";
        }}
    };
    for(int t=0;t<10000;t++) run(1+t%4, 3, 8);
    int sf=fail;
    for(int t=0;t<1000;t++) run(3+t%5, 5, 20);
    cout<<"dp_ac small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}