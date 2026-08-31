#include <bits/stdc++.h>
using namespace std;
const int MOD = 998244353;
int omega[8];
void fwt(int* a, int k, int type) {
  int n_ = 1; // length k^n later passed as N
}
// copy snippet for k-ary? The handbook fwt is k-radix.
// We'll test k=2 (xor FWT-like) by wrapping the snippet with n=power of k.

// Use the actual fwt from file for k=2
void fwt_real(int* a, int k, int type, int N) {
  int tmp[8];
  for (int i = 1; i < N; i *= k)
  for (int j = 0; j < N; j += i * k)
  for (int low = 0; low < i; low++) {
    for (int t = 0; t < k; t++)
      tmp[t] = a[j + t * i + low];
    for (int t = 0; t < k; t++){
      int x = j + t * i + low;
      long long s = 0;
      for (int y = 0; y < k; y++)
        s += 1ll * tmp[y] * omega[(k + type) * t * y % k] % MOD;
      a[x] = int((s % MOD + MOD) % MOD);
    }
  }
  if (type == -1) {
    // inv: divide by k^{log}
  }
}
int qpow(int a,int e){ int r=1; for(;e;e>>=1,a=1ll*a*a%MOD) if(e&1) r=1ll*r*a%MOD; return r; }
mt19937 rng(22);
int fail=0;
int main(){
    // k=2, omega[0]=1, omega[1]=-1 for xor
    int k=2; omega[0]=1; omega[1]=MOD-1;
    auto conv=[&](vector<int> A, vector<int> B){
        int n=A.size();
        vector<int> C(n);
        for(int i=0;i<n;i++) for(int j=0;j<n;j++)
            C[i^j]=(C[i^j]+1ll*A[i]*B[j])%MOD;
        return C;
    };
    for(int t=0;t<10000;t++){
        int lg=1+t%3; int n=1<<lg; // 2,4,8
        vector<int> A(n), B(n);
        for(int i=0;i<n;i++) A[i]=rng()%MOD, B[i]=rng()%MOD;
        auto C=conv(A,B);
        vector<int> a=A,b=B;
        fwt_real(a.data(),2,1,n); fwt_real(b.data(),2,1,n);
        vector<int> c(n);
        for(int i=0;i<n;i++) c[i]=1ll*a[i]*b[i]%MOD;
        fwt_real(c.data(),2,-1,n);
        int invn=qpow(n, MOD-2);
        for(int i=0;i<n;i++) c[i]=1ll*c[i]*invn%MOD;
        if(c!=C){ fail++; if(fail<5) cerr<<"xor fwt\n"; }
    }
    int sf=fail;
    for(int t=0;t<1000;t++){
        int n=1<<6; // 64
        vector<int> A(n), B(n);
        for(int i=0;i<n;i++) A[i]=rng()%MOD, B[i]=rng()%MOD;
        auto C=conv(A,B);
        vector<int> a=A,b=B;
        fwt_real(a.data(),2,1,n); fwt_real(b.data(),2,1,n);
        vector<int> c(n);
        for(int i=0;i<n;i++) c[i]=1ll*a[i]*b[i]%MOD;
        fwt_real(c.data(),2,-1,n);
        int invn=qpow(n, MOD-2);
        for(int i=0;i<n;i++) c[i]=1ll*c[i]*invn%MOD;
        if(c!=C) fail++;
    }
    cout<<"dp_fwt small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}
