#include <bits/stdc++.h>
using namespace std;
using LD = long double;
int sgn(LD x){ return fabsl(x)<1e-9L?0:(x<0?-1:1); }
struct p3 {
	LD x, y, z;
	p3(LD x_ = 0, LD y_ = 0, LD z_ = 0) : x(x_), y(y_), z(z_) {}
	LD len2() const { return x*x + y*y + z*z; }
	LD len() const { return sqrtl(len2()); }
};
p3 operator+(p3 a, p3 b) { return {a.x+b.x, a.y+b.y, a.z+b.z}; }
p3 operator-(p3 a, p3 b) { return {a.x-b.x, a.y-b.y, a.z-b.z}; }
LD dot(p3 a, p3 b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
p3 cross(p3 a, p3 b) {
	return {a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x};
}
LD mix(p3 a, p3 b, p3 c) { return dot(cross(a, b), c); }
const LD eps=1e-9L;
mt19937 rnd{120};
struct ConvexHull3D {
	using Face = array<int,3>;
	vector<p3> p;
	vector<Face> face;
	LD volume(Face f, int x) const {
		return mix(p[f[1]]-p[f[0]],p[f[2]]-p[f[0]],p[x]-p[f[0]]);
	}
	Face outward(int a,int b,int c,int inside) const {
		Face f{a,b,c};
		if (sgn(volume(f,inside))>0) swap(f[1],f[2]);
		return f;
	}
	bool build(vector<p3> points) {
		p=move(points), face.clear();
		if (p.size()<4) return false;
		shuffle(p.begin(),p.end(),rnd);
		int n=(int)p.size(), a=0, b=0, c=0, d=0;
		for (int i=1;i<n;++i) if ((p[i]-p[a]).len2()>(p[b]-p[a]).len2()) b=i;
		for (int i=0;i<n;++i) if ((p[i]-p[b]).len2()>(p[a]-p[b]).len2()) a=i;
		LD bc=-1;
		for (int i=0;i<n;++i) { LD t=cross(p[b]-p[a],p[i]-p[a]).len(); if (t>bc) bc=t, c=i; }
		if (bc<=eps) return false;
		LD bd=-1;
		for (int i=0;i<n;++i) { LD t=fabsl(mix(p[b]-p[a],p[c]-p[a],p[i]-p[a])); if (t>bd) bd=t, d=i; }
		if (bd<=eps) return false;
		vector<p3> q{p[a],p[b],p[c],p[d]};
		for (int i=0;i<n;++i) if (i!=a && i!=b && i!=c && i!=d) q.push_back(p[i]);
		p.swap(q);
		face={outward(0,1,2,3),outward(0,3,1,2),
			outward(0,2,3,1),outward(1,3,2,0)};
		for (int x=4;x<n;++x) {
			bool out=0;
			for (Face f:face) if (sgn(volume(f,x))>0) { out=1; break; }
			if (!out) continue;
			set<pair<int,int>> horizon;
			vector<Face> keep;
			for (Face f:face) if (sgn(volume(f,x))>=0) {
				for (int k=0;k<3;++k) {
					pair<int,int> e={f[k],f[(k+1)%3]},rev={e.second,e.first};
					if (horizon.count(rev)) horizon.erase(rev); else horizon.insert(e);
				}
			} else keep.push_back(f);
			if (horizon.empty()) continue;
			for (auto [u,v]:horizon) keep.push_back({u,v,x});
			face.swap(keep);
		}
		return true;
	}
};
int fail=0;
int main(){
    mt19937 rng(121);
    auto rp=[&](int B){ return p3{(LD)((int)rng()%(2*B)-B),(LD)((int)rng()%(2*B)-B),(LD)((int)rng()%(2*B)-B)}; };
    for(int t=0;t<10000;t++){
        int n=4+t%5;
        vector<p3> pts(n);
        for(int i=0;i<n;i++) pts[i]=rp(4);
        // unique
        ConvexHull3D H;
        bool ok=H.build(pts);
        if(!ok){
            // all coplanar/collinear possible
            continue;
        }
        // every point on inner side of every face
        bool bad=0;
        for(auto f:H.face){
            for(int i=0;i<(int)H.p.size();i++){
                if(i==f[0]||i==f[1]||i==f[2]) continue;
                if(sgn(H.volume(f,i))>0) bad=1;
            }
        }
        if(bad) fail++;
        if(H.face.size()<4) fail++;
    }
    int sf=fail;
    for(int t=0;t<1000;t++){
        int n=8+t%8;
        vector<p3> pts(n);
        for(int i=0;i<n;i++) pts[i]=rp(8);
        ConvexHull3D H;
        if(!H.build(pts)) continue;
        for(auto f:H.face) for(int i=0;i<(int)H.p.size();i++){
            if(i==f[0]||i==f[1]||i==f[2]) continue;
            if(sgn(H.volume(f,i))>0) fail++;
        }
    }
    cout<<"dp_hull3d small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}