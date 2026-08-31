#include <bits/stdc++.h>
using namespace std;
const int MAXN = 1<<12;
#define ls (p<<1)
#define rs (p<<1|1)
namespace Li{
	int tag[MAXN];
	double k[MAXN], b[MAXN];
	const double EPS = 1e-10;
	double calc(int i, double x) {return x * k[i] + b[i];}
	void update(int nl, int nr, int l, int r, int p, int x){
		if(l == r) {if(!tag[p] || calc(tag[p], l) < calc(x, l)) tag[p] = x; return;}
		int mid = (l+r)>>1;
		if(nl <= l && r <= nr){
			if(!tag[p]) {tag[p] = x; return;}
			double l1 = calc(tag[p], l), l2 = calc(x, l), r1 = calc(tag[p], r), r2 = calc(x, r);
			if(abs(l1 - l2) < EPS && abs(r1 - r2) < EPS) return;
			if(l1 > l2 && r1 > r2) return;
			if(l2 > l1 && r2 > r1) tag[p] = x;
			if(calc(tag[p], mid) < calc(x, mid)) swap(x, tag[p]);
			if(k[x] > k[tag[p]]) update(nl, nr, mid+1, r, rs, x);
			if(k[x] < k[tag[p]]) update(nl, nr, l, mid, ls, x);
			return;
		}
		if(nl <= mid) update(nl, nr, l, mid, ls, x);
		if(nr > mid) update(nl, nr, mid+1, r, rs, x);
	}
	double query(int pos, int l, int r, int p){
		double cur = tag[p] ? calc(tag[p], pos) : -1e100;
		if(l == r) return cur;
		int mid = (l+r)>>1;
		if(pos <= mid) return max(cur, query(pos, l, mid, ls));
		else return max(cur, query(pos, mid+1, r, rs));
	}
}
mt19937 rng(16);
int fail=0;
int main(){
    const int L=1, R=20;
    auto run=[&](int nlines, int nq){
        memset(Li::tag,0,sizeof Li::tag);
        int nid=0;
        vector<pair<double,double>> lines;
        lines.push_back({0,0}); // 1-index
        for(int i=0;i<nlines;i++){
            double k = (int)(rng()%21)-10;
            double b = (int)(rng()%41)-20;
            int id=++nid;
            Li::k[id]=k; Li::b[id]=b;
            lines.push_back({k,b});
            int nl=L+rng()%(R-L+1), nr=L+rng()%(R-L+1); if(nl>nr) swap(nl,nr);
            Li::update(nl,nr,L,R,1,id);
            // brute store segments
            static vector<int> cov[21];
            if(i==0) for(int x=L;x<=R;x++) cov[x].clear();
            if(i==0){
                // reset coverage each full run
            }
        }
    };
    // rewrite run with coverage
    auto run2=[&](int nlines){
        memset(Li::tag,0,sizeof Li::tag);
        int nid=0;
        vector<vector<int>> cov(R+1);
        vector<double> kk(nlines+2), bb(nlines+2);
        for(int i=0;i<nlines;i++){
            double k = (int)(rng()%21)-10;
            double b = (int)(rng()%41)-20;
            int id=++nid; Li::k[id]=k; Li::b[id]=b; kk[id]=k; bb[id]=b;
            int nl=L+rng()%(R-L+1), nr=L+rng()%(R-L+1); if(nl>nr) swap(nl,nr);
            Li::update(nl,nr,L,R,1,id);
            for(int x=nl;x<=nr;x++) cov[x].push_back(id);
            for(int x=L;x<=R;x++){
                double got=Li::query(x,L,R,1);
                double bf=-1e100;
                for(int id2: cov[x]) bf=max(bf, kk[id2]*x+bb[id2]);
                if(got<-1e50 && bf<-1e50) continue;
                if(fabs(got-bf)>1e-6){ fail++; if(fail<5) cerr<<"x="<<x<<" got="<<got<<" bf="<<bf<<"\n"; }
            }
        }
    };
    for(int t=0;t<10000;t++) run2(1+t%6);
    int sf=fail;
    for(int t=0;t<1000;t++) run2(8+t%12);
    cout<<"dp_lichao small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}
