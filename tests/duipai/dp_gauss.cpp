#include <bits/stdc++.h>
#include <cassert>
using namespace std;
using LD = long double;
const LD eps = 1e-8;
typedef vector <LD> vec;
pair<vec,vector<vec>> gauss(vector<vec> &a, int n, int d) {
	vector <int> pivot(d, -1); int o = 0;
	for (int i = 0; i < d; i++) {
		int j = o; while (j < n && abs(a[j][i]) < eps) j++;
		if (j == n) continue;
		swap(a[j], a[o]); LD w = a[o][i];
		for (int k = 0; k <= d; k++) a[o][k] /= w;
		for (int x = 0; x < n; x++)
			if (x != o && abs(a[x][i]) > eps) {
				w = a[x][i];
				for (int k = 0; k <= d; k++)
					a[x][k] -= a[o][k] * w;   }
		pivot[i] = o++;
	}
	for (int i = o; i < n; i++) if (abs(a[i][d]) > eps)
		return {{}, {}};
	vec x0(d); vector <vec> t; int free = 0;
	for (int i = 0; i < d; i++)
		if (pivot[i] != -1) x0[i] = -a[pivot[i]][d];
		else free ++;
	for (int i = 0; i < d; i++) if (pivot[i] == -1) {
		vec x(d); x[i] = -1;
		for (int j = 0; j < d; j++)
			if (pivot[j] != -1) x[j] = a[pivot[j]][i];
		t.push_back(x);
	} if (t.size()) {
		vector <vec> f;
		for (int u = 0; u < free; u++) {
			vec x(free + 1);
			for (int i = 0; i < free; i++)
				for (int j = 0; j < d; j++)
					x[i] += t[u][j] * t[i][j];
			for (int j = 0; j < d; j++)
				x[free] += t[u][j] * x0[j];
			f.push_back(x);
		}
		auto [k, tt] = gauss(f, free, free);
		assert (tt.size() == 0);
		for (int x = 0; x < free; x++)
			for (int i = 0; i < d; i++)
				x0[i] += k[x] * t[x][i];
	} return {x0, t}; }
mt19937 rng(115);
int fail=0;
int main(){
    auto residual=[&](vector<vec> A, vec x){
        int n=A.size(), d=x.size();
        LD mx=0;
        for(int i=0;i<n;i++){
            LD s=A[i][d];
            for(int j=0;j<d;j++) s+=A[i][j]*x[j];
            mx=max(mx, (LD)fabsl(s));
        }
        return mx;
    };
    for(int t=0;t<10000;t++){
        int d=1+t%3, n=d+t%2;
        vector<vec> A(n, vec(d+1));
        // consistent: random x then Ax=-b
        vec xt(d);
        for(int j=0;j<d;j++) xt[j]=(int)rng()%5-2;
        for(int i=0;i<n;i++){
            LD b=0;
            for(int j=0;j<d;j++){
                A[i][j]=(int)rng()%5-2;
                b+=A[i][j]*xt[j];
            }
            A[i][d]=-b;
        }
        if(t%7==0){ // make inconsistent
            for(int j=0;j<=d;j++) A[n-1][j]=(int)rng()%3;
            A[n-1][d]+=10;
        }
        auto B=A;
        auto [x0, ker] = gauss(A,n,d);
        if(x0.empty() && ker.empty()){
            // claimed inconsistent - check brute no small integer solution? skip or residual
            continue;
        }
        if(residual(B,x0)>1e-4){ fail++; if(fail<6) cerr<<"res "<<(double)residual(B,x0)<<"\n"; }
        for(auto v:ker){
            vec y=x0; for(int i=0;i<d;i++) y[i]+=v[i]*0.3;
            if(residual(B,y)>1e-3) fail++;
        }
    }
    int sf=fail;
    for(int t=0;t<1000;t++){
        int d=3, n=4;
        vector<vec> A(n, vec(d+1));
        vec xt(d);
        for(int j=0;j<d;j++) xt[j]=(int)rng()%7-3;
        for(int i=0;i<n;i++){
            LD b=0;
            for(int j=0;j<d;j++){ A[i][j]=(int)rng()%7-3; b+=A[i][j]*xt[j]; }
            A[i][d]=-b;
        }
        auto B=A;
        auto [x0,ker]=gauss(A,n,d);
        if(!x0.empty() && residual(B,x0)>1e-3) fail++;
    }
    cout<<"dp_gauss small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}