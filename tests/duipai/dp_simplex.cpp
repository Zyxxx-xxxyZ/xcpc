#include <bits/stdc++.h>
using namespace std;
using LD = long double;
const LD eps = 1e-9, INF = 1e9; const int N = 105;
namespace Simplex {
int n, m, id[N * 2], tp[N * 2]; LD a[N][N];
void pivot(int r, int c) {
	swap(id[r + n], id[c]);
	LD t = -a[r][c]; a[r][c] = -1;
	for (int i = 0; i <= n; i++) a[r][i] /= t;
	for (int i = 0; i <= m; i++) if (a[i][c] && r != i) {
		t = a[i][c]; a[i][c] = 0;
		for (int j = 0; j <= n; j++) a[i][j] += t*a[r][j];}}
bool solve() {
	for (int i = 1; i <= n; i++) id[i] = i;
	for ( ; ; ) {
		int i = 0, j = 0; LD w = -eps;
		for (int k = 1; k <= m; k++)
			if (a[k][0] < w || (a[k][0] < -eps && rand() & 1))
				w = a[i = k][0];
		if (!i) break;
		for (int k = 1; k <= n; k++)
			if (a[i][k] > eps) {j = k; break;}
		if (!j) return 0;
		pivot(i, j);}
	for ( ; ; ) {
		int i = 0, j = 0; LD w = eps, t;
		for (int k = 1; k <= n; k++)
			if (a[0][k] > w) w = a[0][j = k];
		if (!j) break;
		w = INF;
		for (int k = 1; k <= m; k++)
			if (a[k][j] < -eps && (t = -a[k][0]/a[k][j]) < w)
				w = t, i = k;
		if (!i) return 0;
		pivot(i, j);}
	return 1;}
LD ans() {return a[0][0];}
}
mt19937 rng(125);
int fail=0;
int main(){
    using Simplex::n; using Simplex::m; using Simplex::a;
    for(int t=0;t<10000;t++){
        n=1; m=1;
        memset(a,0,sizeof a);
        LD c=rng()%5, A=1+rng()%4, b=rng()%8;
        a[0][1]=c; a[1][1]=-A; a[1][0]=b;
        bool ok=Simplex::solve();
        LD opt = (c>0 ? c*(b/A) : 0);
        if(!ok || fabsl(Simplex::ans()-opt)>1e-4){
            fail++;
            if(fail<6) cerr<<"c="<<(double)c<<" A="<<(double)A<<" b="<<(double)b<<" ok="<<ok<<" z="<<(double)Simplex::ans()<<" opt="<<(double)opt<<"\n";
        }
    }
    int sf=fail;
    // stronger: tiny n=1,m=1, keep copy of c,A,b
    for(int t=0;t<1000;t++){
        n=1; m=1;
        memset(a,0,sizeof a);
        LD c=rng()%5, A=1+rng()%4, b=rng()%8;
        a[0][1]=c; a[1][1]=-A; a[1][0]=b;
        bool ok=Simplex::solve();
        LD opt = (c>0? c*(b/A) : 0);
        if(!ok) fail++;
        else if(fabsl(Simplex::ans()-opt)>1e-4 && c>=0) fail++;
    }
    cout<<"dp_simplex small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}