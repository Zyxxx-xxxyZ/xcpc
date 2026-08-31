#include <bits/stdc++.h>
using namespace std;
using LD = long double;
int sgn(LD x){ return fabsl(x)<1e-12L?0:(x<0?-1:1); }
struct P { LD x,y; };
using cp=const P&;
bool operator<(cp a,cp b){ return a.x==b.x?a.y<b.y:a.x<b.x; }
LD operator^(cp a,cp b){ return a.x*b.y-a.y*b.x; }
P operator-(cp a,cp b){return {a.x-b.x,a.y-b.y};}
int turn(cp a,cp b,cp c){ return sgn((b-a)^(c-a)); }
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
vector<P> brute_upper(vector<P> v){
    // unique max y per x, then monotone chain upper
    map<LD,LD> mx;
    for(auto p:v) mx[p.x]=max(mx.count(p.x)?mx[p.x]:(LD)-1e300, p.y);
    vector<P> pts;
    for(auto [x,y]:mx) pts.push_back({x,y});
    sort(pts.begin(),pts.end());
    vector<P> h;
    for(auto p:pts){
        while(h.size()>=2 && turn(h[h.size()-2], h.back(), p)>=0) h.pop_back(); // upper: turn <=0 pop? 
        // upper hull left to right: should be clockwise, turn < 0 is right turn keep?
        // handbook: keep while turn(*prev(o),*o,p) < 0 break, else erase
        // erase if NOT strict right turn, i.e. turn>=0 (left or collinear)
        // wait: `if (turn(*prev(o), *o, p) < 0) break; erase(o)`
        // so keep going (erase) while turn >= 0, stop at strict right (turn<0)
        // that's lower hull if y-up... turn>0 is CCW left.
        // For upper hull left-to-right, going rightward, the hull should turn right (CW), turn<0.
        // They erase when turn>=0 (not right). Same as: while turn>=0 pop.
        h.push_back(p);
    }
    // rebuild properly:
    h.clear();
    for(auto p:pts){
        while(h.size()>=2 && turn(h[h.size()-2], h.back(), p) >= 0) h.pop_back();
        h.push_back(p);
    }
    return h;
}
mt19937 rng(112);
int fail=0;
int main(){
    auto run=[&](int n, int B){
        hull h;
        vector<P> pts;
        for(int i=0;i<n;i++){
            P p{(LD)((int)rng()%(2*B+1)-B), (LD)((int)rng()%(2*B+1)-B)};
            h.insert(p); pts.push_back(p);
            auto bf=brute_upper(pts);
            vector<P> got(h.a.begin(), h.a.end());
            if(got.size()!=bf.size()){ fail++; if(fail<5) cerr<<"sz "<<got.size()<<" "<<bf.size()<<"\n"; }
            else {
                for(size_t j=0;j<got.size();j++)
                    if(got[j].x!=bf[j].x || got[j].y!=bf[j].y){ fail++; break; }
            }
        }
    };
    for(int t=0;t<10000;t++) run(3+t%6, 5);
    int sf=fail;
    for(int t=0;t<1000;t++) run(10+t%20, 20);
    cout<<"dp_hull small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}