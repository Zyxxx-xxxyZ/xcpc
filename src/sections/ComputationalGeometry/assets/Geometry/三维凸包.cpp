// 随机增量；输入需不存在重复点且仿射维数为 3。
// 点数不足、共线或全部共面时 build 返回 false，face 为空。
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
			if (!out) continue; // inside or in the interior of a face
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
