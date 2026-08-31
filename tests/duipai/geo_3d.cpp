#include "geo_handbook.hpp"
// 三维基础、三维凸包、最小覆盖球 vs 暴力 / 独立公式。

struct p3 {
	LD x, y, z;
	p3(LD x_ = 0, LD y_ = 0, LD z_ = 0) : x(x_), y(y_), z(z_) {}
	LD &operator[](int i) { return i == 0 ? x : (i == 1 ? y : z); }
	LD operator[](int i) const { return i == 0 ? x : (i == 1 ? y : z); }
	LD len2() const { return x*x + y*y + z*z; }
	LD len() const { return sqrtl(len2()); }
	p3 unit() const { LD d = len(); return {x/d, y/d, z/d}; }
};
p3 operator+(p3 a, p3 b) { return {a.x+b.x, a.y+b.y, a.z+b.z}; }
p3 operator-(p3 a, p3 b) { return {a.x-b.x, a.y-b.y, a.z-b.z}; }
p3 operator-(p3 a) { return {-a.x, -a.y, -a.z}; }
p3 operator*(p3 a, LD k) { return {a.x*k, a.y*k, a.z*k}; }
p3 operator*(LD k, p3 a) { return a*k; }
p3 operator/(p3 a, LD k) { return {a.x/k, a.y/k, a.z/k}; }
bool operator==(p3 a, p3 b) {
	return !sgn(a.x-b.x) && !sgn(a.y-b.y) && !sgn(a.z-b.z);
}
LD dot(p3 a, p3 b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
p3 cross(p3 a, p3 b) {
	return {a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x};
}
LD mix(p3 a, p3 b, p3 c) { return dot(cross(a, b), c); }

LD rotation[3][3];
void rotation_matrix(p3 axis, LD angle) {
	axis = axis.unit();
	LD c = cosl(angle), s = sinl(angle);
	for (int i = 0; i < 3; ++i) {
		int j = (i + 1) % 3, k = (i + 2) % 3;
		LD x = axis[i], y = axis[j], z = axis[k];
		rotation[i][i] = (y*y + z*z)*c + x*x;
		rotation[i][j] = x*y*(1-c) - z*s;
		rotation[i][k] = x*z*(1-c) + y*s;
	}
}

struct l3 { p3 s, t; };
struct plane {
	p3 normal; LD offset;
	plane(p3 normal_ = {1,0,0}, p3 point = {})
		: normal(normal_.unit()), offset(dot(normal, point)) {}
};
p3 project_to_plane(p3 a, const plane &b) {
	return a + b.normal * (b.offset - dot(a, b.normal));
}
pair<p3, p3> closest_points(l3 x, l3 y) {
	p3 u = x.t-x.s, v = y.t-y.s, r = x.s-y.s;
	LD a = dot(u,u), b = dot(u,v), e = dot(v,v), d = a*e-b*b;
	LD c = dot(u,r), f = dot(v,r);
	LD s = (b*f-c*e)/d, t = (a*f-b*c)/d;
	return {x.s+u*s, y.s+v*t};
}
p3 line_plane_intersection(const plane &a, l3 b) {
	LD t = (a.offset-dot(a.normal,b.s))/dot(a.normal,b.t-b.s);
	return b.s+(b.t-b.s)*t;
}
l3 plane_plane_intersection(const plane &a, const plane &b) {
	p3 d = cross(a.normal,b.normal);
	LD d2 = d.len2();
	p3 s = cross(b.normal*a.offset-a.normal*b.offset,d)/d2;
	return {s,s+d};
}
p3 three_plane_intersection(const plane &a, const plane &b, const plane &c) {
	LD d = mix(a.normal,b.normal,c.normal);
	return (cross(b.normal,c.normal)*a.offset
		+ cross(c.normal,a.normal)*b.offset
		+ cross(a.normal,b.normal)*c.offset)/d;
}

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

struct Sphere { p3 c; LD r; };
bool in_sphere(p3 p,const Sphere &s) {return sgn((p-s.c).len()-s.r)<=0;}
Sphere sphere(p3 a) {return {a,0};}
Sphere sphere(p3 a,p3 b) {p3 c=(a+b)/2;return {c,(a-c).len()};}
Sphere sphere3(p3 a,p3 b,p3 c) {
	p3 u=b-a,v=c-a,n=cross(u,v);
	if (!sgn(n.len2())) {
		Sphere s=sphere(a,b);
		array<pair<p3,p3>,2> candidate{{{a,c},{b,c}}};
		for (auto [x,y]:candidate) {
			Sphere t=sphere(x,y);
			if (t.r>s.r) s=t;
		}
		return s;
	}
	p3 o=a+(cross(n,u)*v.len2()+cross(v,n)*u.len2())/(2*n.len2());
	return {o,(a-o).len()};
}
Sphere sphere4(p3 a,p3 b,p3 c,p3 d) {
	p3 u=b-a,v=c-a,w=d-a;
	LD det=mix(u,v,w);
	if (!sgn(det)) {
		array<p3,4> q{a,b,c,d}; Sphere best{{},INF};
		for (int i=0;i<4;++i) for (int j=i+1;j<4;++j) {
			Sphere s=sphere(q[i],q[j]); bool ok=true;
			for (p3 x:q) ok&=in_sphere(x,s);
			if (ok&&s.r<best.r) best=s;
		}
		for (int i=0;i<4;++i) {
			Sphere s=sphere3(q[(i+1)%4],q[(i+2)%4],q[(i+3)%4]); bool ok=true;
			for (p3 x:q) ok&=in_sphere(x,s);
			if (ok&&s.r<best.r) best=s;
		}
		return best;
	}
	p3 o=a+(cross(v,w)*(u.len2()/2)+cross(w,u)*(v.len2()/2)
		+cross(u,v)*(w.len2()/2))/det;
	return {o,(a-o).len()};
}
Sphere min_sphere(vector<p3> p) {
	if (p.empty()) return {{},0};
	shuffle(p.begin(),p.end(),rnd); Sphere ret=sphere(p[0]);
	for (int i=1;i<(int)p.size();++i) if (!in_sphere(p[i],ret)) {
		ret=sphere(p[i]);
		for (int j=0;j<i;++j) if (!in_sphere(p[j],ret)) {
			ret=sphere(p[i],p[j]);
			for (int k=0;k<j;++k) if (!in_sphere(p[k],ret)) {
				ret=sphere3(p[i],p[j],p[k]);
				for (int l=0;l<k;++l) if (!in_sphere(p[l],ret))
					ret=sphere4(p[i],p[j],p[k],p[l]);
			}
		}
	}
	return ret;
}

p3 applyR(p3 v) {
	p3 r;
	for (int i = 0; i < 3; ++i)
		r[i] = rotation[i][0]*v[0] + rotation[i][1]*v[1] + rotation[i][2]*v[2];
	return r;
}
p3 rodrigues(p3 k, p3 v, LD ang) {
	k = k.unit();
	LD c = cosl(ang), s = sinl(ang);
	return v*c + cross(k,v)*s + k*dot(k,v)*(1-c);
}

mt19937 rng(311);
int fail = 0;
void oops(const string &s) {
	if (fail < 12) cerr << "geo_3d FAIL " << s << "\n";
	++fail;
}
int irnd(int a, int b) { return uniform_int_distribution<int>(a, b)(rng); }
p3 rp(int B) { return {(LD)irnd(-B,B),(LD)irnd(-B,B),(LD)irnd(-B,B)}; }

bool covers(const Sphere &s, const vector<p3> &pts) {
	for (auto p : pts) if (!in_sphere(p, s)) return false;
	return true;
}

Sphere brute_min_sphere(const vector<p3> &pts) {
	Sphere best{{}, INF};
	int n = (int)pts.size();
	auto ck = [&](Sphere s) {
		if (!isfinite((double)s.r) || s.r < 0) return;
		if (covers(s, pts) && s.r < best.r) best = s;
	};
	for (int i = 0; i < n; ++i) ck(sphere(pts[i]));
	for (int i = 0; i < n; ++i) for (int j = i+1; j < n; ++j) ck(sphere(pts[i], pts[j]));
	for (int i = 0; i < n; ++i) for (int j = i+1; j < n; ++j) for (int k = j+1; k < n; ++k)
		ck(sphere3(pts[i], pts[j], pts[k]));
	if (n <= 8)
		for (int i = 0; i < n; ++i) for (int j = i+1; j < n; ++j)
			for (int k = j+1; k < n; ++k) for (int l = k+1; l < n; ++l)
				ck(sphere4(pts[i], pts[j], pts[k], pts[l]));
	return best;
}

int main() {
	for (int t = 0; t < 10000; ++t) {
		p3 k = rp(5); if (k.len() < 1e-6) k = {1,0,0};
		LD ang = (LD)irnd(-30, 30) / 10;
		p3 v = rp(6);
		rotation_matrix(k, ang);
		p3 a = applyR(v), b = rodrigues(k, v, ang);
		if (fabsl(a.x-b.x)+fabsl(a.y-b.y)+fabsl(a.z-b.z) > 1e-6L) oops("rot");
		if (fabsl(a.len() - v.len()) > 1e-6L) oops("rot_len");

		p3 nrm{1,0,0}; if (t%3==1) nrm={0,1,0}; if (t%3==2) nrm={0,0,1};
		plane pl(nrm, {0,0,0});
		p3 q = rp(5);
		p3 pr = project_to_plane(q, pl);
		if (fabsl(dot(pr, pl.normal) - pl.offset) > 1e-8L) oops("proj_plane");

		l3 L1{{0,0,0},{1,0,0}}, L2{{0,1,1},{0,1,2}};
		auto cpairs = closest_points(L1, L2);
		if (fabsl(dot(cpairs.first - cpairs.second, L1.t-L1.s)) > 1e-6L) oops("skew1");
		if (fabsl(dot(cpairs.first - cpairs.second, L2.t-L2.s)) > 1e-6L) oops("skew2");

		vector<p3> pts;
		set<array<int,3>> st;
		int n = 4 + t % 4;
		for (int i = 0; i < n * 4 && (int)pts.size() < n; ++i) {
			p3 p = rp(3);
			if (st.insert({(int)p.x,(int)p.y,(int)p.z}).second) pts.push_back(p);
		}
		if ((int)pts.size() >= 4) {
			ConvexHull3D H;
			if (H.build(pts)) {
				for (auto f : H.face) for (int i = 0; i < (int)H.p.size(); ++i) {
					if (i==f[0]||i==f[1]||i==f[2]) continue;
					if (sgn(H.volume(f, i)) > 0) oops("hull3d_out");
				}
			}
			Sphere g = min_sphere(pts);
			if (!isfinite((double)g.r)) oops("ms_nan");
			else if (!covers(g, pts)) oops("ms_cover");
			else {
				Sphere bf = brute_min_sphere(pts);
				if (g.r > bf.r + 1e-5L) oops("ms_not_min");
			}
		}
		if (fail > 40) break;
	}
	int sf = fail;
	for (int t = 0; t < 1000; ++t) {
		vector<p3> pts;
		set<array<int,3>> st;
		int n = 8 + t % 6;
		for (int i = 0; i < n * 4 && (int)pts.size() < n; ++i) {
			p3 p = rp(8);
			if (st.insert({(int)p.x,(int)p.y,(int)p.z}).second) pts.push_back(p);
		}
		ConvexHull3D H;
		if (H.build(pts)) {
			for (auto f : H.face) for (int i = 0; i < (int)H.p.size(); ++i)
				if (i!=f[0]&&i!=f[1]&&i!=f[2] && sgn(H.volume(f,i))>0) oops("hull3d_large");
		}
		Sphere g = min_sphere(pts);
		if (!covers(g, pts)) oops("ms_cover_large");
		if (fail > 80) break;
	}
	cout << "geo_3d small_fail=" << sf << " total_fail=" << fail << (fail ? " FAIL\n" : " OK\n");
	return fail ? 1 : 0;
}
