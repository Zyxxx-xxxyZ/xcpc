#include "geo_handbook.hpp"
// Ear clipping、动态上凸包、Delaunay 空圆 vs 暴力。

struct hull {
set<P> a; LD tot;
hull () {tot = 0;}
template<class It> LD calc(It it) {
	auto u = it == a.begin() ? a.end() : prev(it);
	auto v = next(it);
	LD ret = 0;
	if (u != a.end()) ret += *u ^ *it;
	if (v != a.end()) ret += *it ^ *v;
	if (u != a.end() && v != a.end()) ret -= *u ^ *v;
	return ret; }
void insert (P p) {
	if (!a.size()) { a.insert (p); return; }
	auto it = a.lower_bound(p);
	if (it != a.end() && it->x == p.x) {
		if (p.y <= it->y) return;
		erase(it); it = a.lower_bound(p); }
	else if (it != a.begin() && prev(it)->x == p.x) {
		auto o = prev(it);
		if (p.y <= o->y) return;
		erase(o); it = a.lower_bound(p); }
	bool out = it == a.begin() || it == a.end()
		|| turn(*prev(it), *it, p) > 0;
	if (!out) return;
	while (a.size() >= 2 && it != a.begin()) {
		auto o = prev(it);
		if (o == a.begin()) break;
		if (turn(*prev(o), *o, p) < 0) break;
		erase(o); it = a.lower_bound(p); }
	while (it != a.end()) {
		auto o = next(it);
		if (o == a.end() || turn(p, *it, *o) < 0) break;
		else erase(it), it = o; }
	tot += calc(a.insert(p).first); }
template<class It> void erase(It it) { tot -= calc(it); a.erase(it); } };

vector<array<int, 3>> ear_clipping(cvp p) {
	int n = (int)p.size();
	if (n < 3) return {};
	vector<int> id(n);
	iota(id.begin(), id.end(), 0);
	if (polygon_signed_area2(p) < 0) reverse(id.begin(), id.end());
	vector<array<int, 3>> ret;
	for (bool changed = true; changed && id.size() > 3; ) {
		changed = false;
		for (int i = 0, m = (int)id.size(); i < m; ++i) {
			int a = id[(i + m - 1) % m], b = id[i], c = id[(i + 1) % m];
			if (!turn(p[a], p[b], p[c]) && point_on_segment(p[b], {p[a], p[c]})) {
				id.erase(id.begin() + i), changed = true;
				break;
			}
		}
	}

	auto in_triangle = [&](cp q, cp a, cp b, cp c) {
		return turn(a, b, q) >= 0 && turn(b, c, q) >= 0
			&& turn(c, a, q) >= 0;
	};
	while (id.size() > 3) {
		bool found = false;
		for (int i = 0, m = (int)id.size(); i < m; ++i) {
			int a = id[(i + m - 1) % m], b = id[i], c = id[(i + 1) % m];
			if (turn(p[a], p[b], p[c]) <= 0) continue;
			bool empty = true;
			for (int x : id) if (x != a && x != b && x != c
				&& in_triangle(p[x], p[a], p[b], p[c])) {
				empty = false;
				break;
			}
			if (!empty) continue;
			ret.push_back({a, b, c});
			id.erase(id.begin() + i);
			found = true;
			break;
		}
		if (!found) return {};
	}
	ret.push_back({id[0], id[1], id[2]});
	return ret;
}

bool in_circum(cp p1, cp p2, cp p3, cp p4) {
 LD u11 = p1.x-p4.x, u21 = p2.x-p4.x, u31 = p3.x-p4.x;
 LD u12 = p1.y-p4.y, u22 = p2.y-p4.y, u32 = p3.y-p4.y;
 LD u13 = p1.len2()-p4.len2(), u23 = p2.len2()-p4.len2();
 LD u33 = p3.len2()-p4.len2();
 LD d = -u13*u22*u31 + u12*u23*u31 + u13*u21*u32
	- u11*u23*u32 - u12*u21*u33 + u11*u22*u33;
 return sgn(d) > 0; }
LD dir(cp a, cp b, cp p) { return (b-a) ^ (p-a);}
typedef int SideRef; struct Tri; typedef Tri* TriRef;
struct Edge {
	TriRef tri; SideRef side; Edge() : tri(0), side(0) {}
	Edge(TriRef tri_, SideRef side_) : tri(tri_), side(side_) {} };
const int N = 200, MAX_TRIS = N * 6;
struct Tri {
	P p[3];Edge edge[3];TriRef ch[3]; Tri(){}
	Tri(cp p0,cp p1,cp p2){
		p[0] = p0; p[1] = p1; p[2] = p2;
				ch[0] = ch[1] = ch[2] = 0; }
	bool has_ch() const { return ch[0] != 0; }
	bool contains(cp q) const {
		LD a=dir(p[0],p[1],q), b=dir(p[1],p[2],q), c=dir(p[2],p[0],q);
		return sgn(a) >= 0 && sgn(b) >= 0 && sgn(c) >= 0; }
} triange_pool[MAX_TRIS], *tot_tri;
void set_edge(Edge a, Edge b) {
	if (a.tri) a.tri->edge[a.side] = b;
	if (b.tri) b.tri->edge[b.side] = a; }
class Triangulation {
	public:
		Triangulation() {
			tot_tri = triange_pool;
			const LD LOTS = 1e6;
			the_root = new(tot_tri++) Tri (P(-LOTS,-LOTS), P(+LOTS,-LOTS), P(0,+LOTS)); }
		void add_point(cp p) { add_point(find(the_root,p),p); }
		void build(vp p) {
			shuffle(p.begin(), p.end(), rnd);
			for (cp x : p) add_point(x);
		}
		vector<Tri*> leaves(){
			vector<Tri*> r;
			function<void(Tri*)> dfs=[&](Tri* t){
				if(!t->has_ch()) r.push_back(t);
				else for(int i=0;i<3;i++) if(t->ch[i]) dfs(t->ch[i]);
			};
			dfs(the_root); return r;
		}
	private:
		TriRef the_root;
		static TriRef find(TriRef root,cp p){
			for( ; ; ) {
				if (!root->has_ch()) return root;
				TriRef nxt = 0;
				for (int i = 0; i < 3 && root->ch[i] ; ++i)
					if (root->ch[i]->contains(p))
						{ nxt = root->ch[i]; break; }
				root = nxt ? nxt : root->ch[0]; } }
		void add_point(TriRef root, cp p) {
			TriRef tab,tbc,tca;
			tab = new(tot_tri++) Tri(root->p[0], root->p[1], p);
			tbc = new(tot_tri++) Tri(root->p[1], root->p[2], p);
			tca = new(tot_tri++) Tri(root->p[2], root->p[0], p);
			set_edge(Edge(tab,0),Edge(tbc,1));
			set_edge(Edge(tbc,0),Edge(tca,1));
			set_edge(Edge(tca,0),Edge(tab,1));
			set_edge(Edge(tab,2),root->edge[2]);
			set_edge(Edge(tbc,2),root->edge[0]);
			set_edge(Edge(tca,2),root->edge[1]);
			root->ch[0]=tab;root->ch[1]=tbc;root->ch[2]=tca;
			flip(tab,2); flip(tbc,2); flip(tca,2); }
		void flip(TriRef tri, SideRef side_index) {
			TriRef trj = tri->edge[side_index].tri;
			int pj = tri->edge[side_index].side;
			if(!trj || !in_circum(tri->p[0],tri->p[1],tri->p[2],trj->p[pj])) return;
			TriRef trk = new(tot_tri++) Tri(tri->p[(side_index+1)%3], trj->p[pj], tri->p[side_index]);
			TriRef trl = new(tot_tri++) Tri(trj->p[(pj+1)%3], tri->p[side_index], trj->p[pj]);
			set_edge(Edge(trk,0), Edge(trl,0));
			set_edge(Edge(trk,1), tri->edge[(side_index+2)%3]);
			set_edge(Edge(trk,2), trj->edge[(pj+1)%3]);
			set_edge(Edge(trl,1), trj->edge[(pj+2)%3]);
			set_edge(Edge(trl,2), tri->edge[(side_index+1)%3]);
			tri->ch[0]=trk; tri->ch[1]=trl; tri->ch[2]=0;
			trj->ch[0]=trk; trj->ch[1]=trl; trj->ch[2]=0;
			flip(trk,1); flip(trk,2); flip(trl,1); flip(trl,2); }
};

mt19937 rng(312);
int fail = 0;
void oops(const string &s) {
	if (fail < 12) cerr << "geo_mesh FAIL " << s << "\n";
	++fail;
}
int irnd(int a, int b) { return uniform_int_distribution<int>(a, b)(rng); }

vp brute_upper(vector<P> v) {
	map<LD, LD> mx;
	for (auto p : v) mx[p.x] = max(mx.count(p.x) ? mx[p.x] : (LD)-1e300, p.y);
	vector<P> pts;
	for (auto [x, y] : mx) pts.push_back({x, y});
	vector<P> h;
	for (auto p : pts) {
		while (h.size() >= 2 && turn(h[h.size() - 2], h.back(), p) >= 0) h.pop_back();
		h.push_back(p);
	}
	return h;
}

int main() {
	for (int t = 0; t < 10000; ++t) {
		int n = 3 + t % 6;
		vector<P> pts;
		hull H;
		for (int i = 0; i < n; ++i) {
			P p{(LD)irnd(-6, 6), (LD)irnd(-6, 6)};
			pts.push_back(p);
			H.insert(p);
			auto bf = brute_upper(pts);
			vector<P> got(H.a.begin(), H.a.end());
			if (got.size() != bf.size()) oops("dynhull_sz");
			else {
				for (size_t j = 0; j < got.size(); ++j)
					if (got[j].x != bf[j].x || got[j].y != bf[j].y) { oops("dynhull"); break; }
			}
		}

		auto poly = convex_hull({
			{(LD)irnd(-5, 0), (LD)irnd(-5, 0)},
			{(LD)irnd(0, 5), (LD)irnd(-5, 0)},
			{(LD)irnd(0, 5), (LD)irnd(0, 5)},
			{(LD)irnd(-5, 0), (LD)irnd(0, 5)},
			{(LD)irnd(-2, 2), (LD)irnd(-2, 2)}
		});
		if (poly.size() >= 3) {
			auto tris = ear_clipping(poly);
			if (!tris.empty()) {
				LD sa = 0;
				for (auto tr : tris)
					sa += fabsl((poly[tr[1]] - poly[tr[0]]) ^ (poly[tr[2]] - poly[tr[0]]));
				if (fabsl(sa - fabsl(polygon_signed_area2(poly))) > 1e-6L) oops("ear_area");
			}
		}

		if (t % 4 == 0) {
			vp dpts;
			set<pair<int,int>> st;
			int m = 3 + t % 5;
			for (int i = 0; i < m * 3 && (int)dpts.size() < m; ++i) {
				int x = irnd(-6, 6), y = irnd(-6, 6);
				if (st.insert({x, y}).second) dpts.push_back({(LD)x, (LD)y});
			}
			if (!dpts.empty()) {
				Triangulation T; T.build(dpts);
				const LD LOTS = 1e6;
				for (auto tri : T.leaves()) {
					bool outer = 0;
					for (int k = 0; k < 3; ++k)
						if (fabsl(tri->p[k].x) > LOTS/2 || fabsl(tri->p[k].y) > LOTS/2) outer = 1;
					if (outer) continue;
					for (auto q : dpts) {
						bool vert = 0;
						for (int k = 0; k < 3; ++k)
							if (sgn(tri->p[k].x - q.x) == 0 && sgn(tri->p[k].y - q.y) == 0) vert = 1;
						if (vert) continue;
						if (in_circum(tri->p[0], tri->p[1], tri->p[2], q)) oops("delaunay");
					}
				}
			}
		}
		if (fail > 40) break;
	}
	int sf = fail;
	for (int t = 0; t < 1000; ++t) {
		vector<P> pts;
		hull H;
		int n = 12 + t % 10;
		for (int i = 0; i < n; ++i) {
			P p{(LD)irnd(-20, 20), (LD)irnd(-20, 20)};
			pts.push_back(p);
			H.insert(p);
		}
		auto bf = brute_upper(pts);
		vector<P> got(H.a.begin(), H.a.end());
		if (got.size() != bf.size()) oops("dynhull_large");
		auto poly = convex_hull(pts);
		if (poly.size() >= 3) {
			auto tris = ear_clipping(poly);
			if (!tris.empty()) {
				LD sa = 0;
				for (auto tr : tris)
					sa += fabsl((poly[tr[1]] - poly[tr[0]]) ^ (poly[tr[2]] - poly[tr[0]]));
				if (fabsl(sa - fabsl(polygon_signed_area2(poly))) > 1e-5L) oops("ear_large");
			}
		}
		if (fail > 80) break;
	}
	cout << "geo_mesh small_fail=" << sf << " total_fail=" << fail << (fail ? " FAIL\n" : " OK\n");
	return fail ? 1 : 0;
}
