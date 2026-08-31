#include <bits/stdc++.h>
using namespace std;
using LD = long double;
int sgn(LD x){ return fabsl(x)<1e-8L?0:(x<0?-1:1); }
struct P { LD x,y; P(LD x_=0,LD y_=0):x(x_),y(y_){} LD len2()const{return x*x+y*y;} };
using cp=const P&; using vp=vector<P>;
P operator-(cp a,cp b){return {a.x-b.x,a.y-b.y};}
LD operator^(cp a,cp b){return a.x*b.y-a.y*b.x;}
mt19937 rnd{127};
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
		void add_point(TriRef root, cp p);
		void flip(TriRef tri, SideRef side_index);
};
void Triangulation::add_point(TriRef root, cp p) {
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
void Triangulation::flip(TriRef tri, SideRef side_index) {
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
	flip(trk,1); flip(trk,2); flip(trl,1); flip(trl,2);
}
int fail=0;
mt19937 rng(128);
int main(){
    auto run=[&](int n, int B){
        vp pts;
        set<pair<int,int>> st;
        for(int i=0;i<n;i++){
            int x=rng()%(2*B+1)-B, y=rng()%(2*B+1)-B;
            if(st.insert({x,y}).second) pts.push_back(P((LD)x,(LD)y));
        }
        if(pts.size()<1) return;
        Triangulation T; T.build(pts);
        auto ls=T.leaves();
        const LD LOTS=1e6;
        for(auto t:ls){
            // skip huge outer if uses LOTS vertices
            bool outer=0;
            for(int k=0;k<3;k++) if(fabsl(t->p[k].x)>LOTS/2 || fabsl(t->p[k].y)>LOTS/2) outer=1;
            if(outer) continue;
            for(auto q:pts){
                bool vert=0;
                for(int k=0;k<3;k++) if(sgn(t->p[k].x-q.x)==0 && sgn(t->p[k].y-q.y)==0) vert=1;
                if(vert) continue;
                if(in_circum(t->p[0],t->p[1],t->p[2],q)) { fail++; return; }
            }
        }
    };
    for(int t=0;t<10000;t++) run(3+t%6, 6);
    int sf=fail;
    for(int t=0;t<1000;t++) run(8+t%8, 12);
    cout<<"dp_delaunay small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}