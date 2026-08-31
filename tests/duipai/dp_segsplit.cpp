#include <bits/stdc++.h>
using namespace std;
const int MAXN=20000;
int ls[MAXN], rs[MAXN], sum[MAXN], tot;
int new_node(){ int x=++tot; ls[x]=rs[x]=sum[x]=0; return x; }
void push_up(int x){ sum[x]=sum[ls[x]]+sum[rs[x]]; }
int build(int l,int r, const vector<int>& a){
    int x=new_node();
    if(l==r){ sum[x]=a[l]; return x; }
    int mid=(l+r)>>1;
    ls[x]=build(l,mid,a); rs[x]=build(mid+1,r,a);
    push_up(x); return x;
}
void split(int& x, int& y, int nl, int nr, int l, int r){
	if(!x) { y = 0; return; }
	if(nl <= l && r <= nr) {y = x, x = 0; return;}
	y = new_node();
	int mid = (l+r)>>1;
	if(nl <= mid) split(ls[x], ls[y], nl, nr, l, mid);
	if(nr > mid) split(rs[x], rs[y], nl, nr, mid+1, r);
	push_up(x), push_up(y);
}
int qsum(int x,int ql,int qr,int l,int r){
    if(!x||qr<l||ql>r) return 0;
    if(ql<=l&&r<=qr) return sum[x];
    int mid=(l+r)>>1;
    return qsum(ls[x],ql,qr,l,mid)+qsum(rs[x],ql,qr,mid+1,r);
}
mt19937 rng(106);
int fail=0;
int main(){
    auto run=[&](int n){
        tot=0;
        vector<int> a(n+1);
        for(int i=1;i<=n;i++) a[i]=rng()%10;
        int rt=build(1,n,a);
        int L=1+rng()%n, R=1+rng()%n; if(L>R) swap(L,R);
        int y=0;
        split(rt,y,L,R,1,n);
        int sL=0,sM=0,sR=0;
        for(int i=1;i<L;i++) sL+=a[i];
        for(int i=L;i<=R;i++) sM+=a[i];
        for(int i=R+1;i<=n;i++) sR+=a[i];
        int gM=sum[y], gRest=sum[rt];
        if(gM!=sM) fail++;
        if(gRest!=sL+sR) fail++;
        // empty split
        int z=0, dummy=0;
        split(dummy,z,1,n,1,n);
        if(z!=0) fail++;
    };
    for(int t=0;t<10000;t++) run(1+t%8);
    int sf=fail;
    for(int t=0;t<1000;t++) run(16+t%48);
    cout<<"dp_segsplit small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}