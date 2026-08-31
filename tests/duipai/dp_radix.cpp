#include <bits/stdc++.h>
using namespace std;
const int SZ = 1 << 8;
int b[SZ], c[20050];
void SORT(int a[], int c[], int n, int w) {
	for(int i=0; i<SZ; i++) b[i] = 0;
	for(int i=1; i<=n; i++) b[(a[i]>>w) & (SZ-1)]++;
	for(int i=1; i<SZ; i++) b[i] += b[i - 1];
	for(int i=n; i; i--) c[b[(a[i]>>w) & (SZ-1)]--] = a[i];}
void Sort(int *a, int n){
	for(int i=1;i<=n;i++) a[i] ^= 1u<<31;
	SORT(a, c, n, 0); SORT(c, a, n, 8);
	SORT(a, c, n, 16); SORT(c, a, n, 24);
	for(int i=1;i<=n;i++) a[i] ^= 1u<<31; }
mt19937 rng(11);
int fail=0;
int main(){
    int a[2050], g[2050];
    for(int t=0;t<10000;t++){
        int n=1+t%20;
        for(int i=1;i<=n;i++) a[i]=g[i]=(int)rng();
        Sort(a,n); sort(g+1,g+n+1);
        for(int i=1;i<=n;i++) if(a[i]!=g[i]) { fail++; break; }
    }
    int sf=fail;
    for(int t=0;t<1000;t++){
        int n=200+t%800;
        vector<int> A(n+1), G(n+1);
        for(int i=1;i<=n;i++) A[i]=G[i]=(int)rng();
        // use heap buffers
        static int aa[20050], cc[20050];
        for(int i=1;i<=n;i++) aa[i]=A[i];
        // local Sort using global c
        auto SORT2=[&](int *src, int *dst, int w){
            int bb[256]={};
            for(int i=1;i<=n;i++) bb[(src[i]>>w)&255]++;
            for(int i=1;i<256;i++) bb[i]+=bb[i-1];
            for(int i=n;i;i--) dst[bb[(src[i]>>w)&255]--]=src[i];
        };
        for(int i=1;i<=n;i++) aa[i]^=1u<<31;
        SORT2(aa,cc,0); SORT2(cc,aa,8); SORT2(aa,cc,16); SORT2(cc,aa,24);
        for(int i=1;i<=n;i++) aa[i]^=1u<<31;
        sort(G.begin()+1, G.end());
        for(int i=1;i<=n;i++) if(aa[i]!=G[i]) { fail++; break; }
    }
    cout<<"dp_radix small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}
