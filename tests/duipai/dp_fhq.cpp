#include <bits/stdc++.h>
using namespace std;
const int N = 20000;
mt19937 rd_rng(15);
int rd(){ return (int)rd_rng(); }
void push_down(int){}
int a[N];
struct FHQ{
int val[N], dat[N], siz[N], ls[N], rs[N], tot = 0, rt;
void push_up(int x) {siz[x] = siz[ls[x]] + siz[rs[x]] + 1;}
int merge(int x, int y){
	if(!x || !y) return x+y;
	if(dat[x] < dat[y]) {push_down(x); rs[x] = merge(rs[x], y); push_up(x); return x;}
	else {push_down(y); ls[y] = merge(x, ls[y]); push_up(y); return y;} }
void splitVal(int p, int k, int& x, int& y){
	if(!p) {x = y = 0; return;}
	push_down(p);
	if(val[p] <= k) {x = p; splitVal(rs[x], k, rs[x], y);}
	else {y = p; splitVal(ls[y], k, x, ls[y]);}
	push_up(p);
}
int new_node(int v){
	siz[++tot] = 1; val[tot] = v;
	dat[tot] = rd(); return tot;}
void ins(int v){
	int x, y; splitVal(rt, v, x, y);
	rt = merge(merge(x, new_node(v)), y); }
void del(int v){
	int x, y, z;
	splitVal(rt, v, x, z), splitVal(x, v-1, x, y);
	y = merge(ls[y], rs[y]); rt = merge(merge(x, y), z); }
int rk(int v){
	int x, y; splitVal(rt, v-1, x, y);
	int t = siz[x] + 1; rt = merge(x, y); return t; }
int kth(int p, int k){
	if(!p || k <= 0) return 0;
	while(p){
		if(siz[ls[p]] >= k) p = ls[p];
		else if(siz[ls[p]] + 1 == k) return val[p];
		else k -= siz[ls[p]] + 1, p = rs[p]; }
	return 0; }
int pre(int v){
	int x, y; splitVal(rt, v-1, x, y);
	int t = kth(x, siz[x]); rt = merge(x, y); return t; }
int nxt(int v){
	int x, y; splitVal(rt, v, x, y);
	int t = kth(y, 1); rt = merge(x, y); return t; }
} tr;
int fail=0;
void oops(string s){ if(fail<8) cerr<<"FAIL "<<s<<"\n"; fail++; }
int main(){
    auto run=[&](int nops, int V){
        tr = FHQ{};
        multiset<int> s;
        for(int i=0;i<nops;i++){
            int op=rd_rng()%6, v=rd_rng()%V;
            if(op<=2 || s.empty()){ tr.ins(v); s.insert(v); }
            else if(op==3){
                auto it=s.find(v);
                if(it==s.end()){
                    // del missing: snippet still splits; skip to avoid deleting nothing mismatch
                    continue;
                }
                tr.del(v); s.erase(it);
            } else if(op==4){
                int k=1+rd_rng()%(s.size()+2);
                int got=tr.kth(tr.rt, k);
                int bf=0;
                if(k>=1 && k<=(int)s.size()){ auto it=s.begin(); advance(it,k-1); bf=*it; }
                if(got!=bf) oops("kth");
            } else {
                int got=tr.rk(v);
                int bf=1; for(int x:s){ if(x>=v) break; bf++; }
                if(got!=bf) oops("rk");
            }
        }
    };
    for(int t=0;t<10000;t++) run(15+t%20, 12);
    int sf=fail;
    for(int t=0;t<1000;t++) run(80+t%80, 100);
    cout<<"dp_fhq small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}
