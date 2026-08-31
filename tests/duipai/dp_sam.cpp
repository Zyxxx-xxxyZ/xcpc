#include <bits/stdc++.h>
using namespace std;
const int MAXN=200;
struct SAM{
	int last = 1, tot = 1;
	int ch[MAXN<<1][26]={}, len[MAXN<<1]={}, f[MAXN<<1]={};
	void ins(char c){
		c -= 'a';
		int p = last, cur = last = ++tot;
		len[cur] = len[p] + 1;
		for(;p && !ch[p][c];p=f[p]) ch[p][c] = cur;
		if(!p) {f[cur] = 1; return;}
		int q = ch[p][c];
		if(len[q] == len[p] + 1) {f[cur] = q; return;}
		int clone = ++tot;
		for(int i=0;i<26;i++) ch[clone][i] = ch[q][i];
		f[clone] = f[q], len[clone] = len[p] + 1;
		f[q] = f[cur] = clone;
		for(;p && ch[p][c] == q;p=f[p]) ch[p][c] = clone;
	}
	void reset(){ last=tot=1; memset(ch,0,sizeof ch); memset(len,0,sizeof len); memset(f,0,sizeof f); }
} sam;
struct GSAM{
	int ch[MAXN<<1][26]={}, f[MAXN<<1]={}, len[MAXN<<1]={}, tot = 1;
	int ins(int c, int last){
		c -= 'a';
		int cur, p;
		if(ch[last][c]){
			p = last, cur = ch[p][c];
			if(len[p] + 1 == len[cur]) return cur;
			int q = cur, clone = ++tot;
			for(int i=0;i<26;i++) ch[clone][i] = ch[q][i];
			len[clone] = len[p] + 1;
			f[clone] = f[q], f[q] = clone;
			for(;p&&ch[p][c]==q;p=f[p]) ch[p][c] = clone;
			return clone;
		}
		p = last, cur = ++tot;
		len[cur] = len[p] + 1;
		for(;p&&!ch[p][c];p=f[p]) ch[p][c] = cur;
		if(!p) {f[cur] = 1; return cur;}
		int q = ch[p][c];
		if(len[p] + 1 == len[q]) {f[cur] = q; return cur;}
		int clone = ++tot;
		for(int i=0;i<26;i++) ch[clone][i] = ch[q][i];
		len[clone] = len[p] + 1;
		f[clone] = f[q], f[q] = f[cur] = clone;
		for(;p&&ch[p][c]==q;p=f[p]) ch[p][c] = clone;
		return cur;
	}
	void ins(char* s){
		int last = 1;
		for(int i=0;s[i];i++) last = ins(s[i], last);
	}
	void reset(){ tot=1; memset(ch,0,sizeof ch); memset(f,0,sizeof f); memset(len,0,sizeof len); }
} gsam;
long long sam_distinct(SAM& s){
    long long ans=0;
    for(int i=2;i<=s.tot;i++) ans += s.len[i]-s.len[s.f[i]];
    return ans;
}
long long gsam_distinct(GSAM& s){
    long long ans=0;
    for(int i=2;i<=s.tot;i++) ans += s.len[i]-s.len[s.f[i]];
    return ans;
}
mt19937 rng(110);
int fail=0;
int main(){
    for(int t=0;t<10000;t++){
        int n=1+t%8;
        string s; for(int i=0;i<n;i++) s+='a'+rng()%3;
        sam.reset();
        for(char c:s) sam.ins(c);
        set<string> st;
        for(int i=0;i<n;i++) for(int j=i;j<n;j++) st.insert(s.substr(i,j-i+1));
        if(sam_distinct(sam)!=(long long)st.size()) fail++;
        // default-init path: SAM tmp; without memset besides ={}
        // generalized
        gsam.reset();
        char buf[32]; memcpy(buf,s.c_str(),s.size()+1);
        gsam.ins(buf);
        if(gsam_distinct(gsam)!=(long long)st.size()) fail++;
    }
    int sf=fail;
    for(int t=0;t<1000;t++){
        int n=10+t%20;
        string s; for(int i=0;i<n;i++) s+='a'+rng()%4;
        sam.reset(); for(char c:s) sam.ins(c);
        set<string> st;
        for(int i=0;i<n;i++){ string cur; for(int j=i;j<n;j++){ cur+=s[j]; st.insert(cur);} }
        if(sam_distinct(sam)!=(long long)st.size()) fail++;
        // two strings generalized
        string s2; int n2=3+t%8; for(int i=0;i<n2;i++) s2+='a'+rng()%4;
        gsam.reset();
        char b1[64], b2[64];
        memcpy(b1,s.c_str(),s.size()+1); memcpy(b2,s2.c_str(),s2.size()+1);
        gsam.ins(b1); gsam.ins(b2);
        set<string> st2;
        auto add=[&](const string& u){ for(size_t i=0;i<u.size();i++){ string c; for(size_t j=i;j<u.size();j++){ c+=u[j]; st2.insert(c);} } };
        add(s); add(s2);
        if(gsam_distinct(gsam)!=(long long)st2.size()) fail++;
    }
    cout<<"dp_sam small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}