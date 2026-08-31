#include <bits/stdc++.h>
using namespace std;
using LD = long double;
int sgn(LD x){ return fabsl(x)<1e-12L?0:(x<0?-1:1); }
struct P { LD x,y; };
using cp=const P&; using vp=vector<P>;
bool operator<(cp a,cp b){ return a.x==b.x?a.y<b.y:a.x<b.x; }
bool operator==(cp a,cp b){ return a.x==b.x && a.y==b.y; }
P operator+(cp a,cp b){return {a.x+b.x,a.y+b.y};}
P operator-(cp a,cp b){return {a.x-b.x,a.y-b.y};}
LD operator^(cp a,cp b){return a.x*b.y-a.y*b.x;}
int half(cp a) { return a.y > 0 || (a.y == 0 && a.x > 0); }
int turn(cp a,cp b,cp c){ return sgn((b-a)^(c-a)); }
vp minkowski_sum(vp a, vp b) {
	auto yx = [](cp u, cp v) { return u.y != v.y ? u.y < v.y : u.x < v.x; };
	if (!a.empty()) rotate(a.begin(), min_element(a.begin(), a.end(), yx), a.end());
	if (!b.empty()) rotate(b.begin(), min_element(b.begin(), b.end(), yx), b.end());
	if (a.size() == 1 || b.size() == 1) {
		vp ret;
		for (auto i : a) for (auto j : b) ret.push_back(i+j);
		return ret; }
	vp x, y;
	for (size_t i = 0; i < a.size(); ++i)
		x.push_back(a[(i + 1) % a.size()] - a[i]);
	for (size_t i = 0; i < b.size(); ++i)
		y.push_back(b[(i + 1) % b.size()] - b[i]);
	vp ret (x.size() + y.size());
	merge(x.begin(), x.end(), y.begin(), y.end(),
		  ret.begin(), [](cp u, cp v) {
		return half(u) != half(v) ? half(u) > half(v) : (u ^ v) > 0;});
	P cur = a[0] + b[0];
	for (auto &i : ret) swap(i, cur), cur = cur + i;
	return ret; }
vp convex(vp v){
    sort(v.begin(),v.end());
    v.erase(unique(v.begin(),v.end(),[](P a,P b){return a.x==b.x&&a.y==b.y;}), v.end());
    if(v.size()<=1) return v;
    vp lo, up;
    for(auto p:v){
        while(lo.size()>=2 && turn(lo[lo.size()-2],lo.back(),p)<=0) lo.pop_back();
        lo.push_back(p);
    }
    for(int i=(int)v.size()-1;i>=0;i--){
        auto p=v[i];
        while(up.size()>=2 && turn(up[up.size()-2],up.back(),p)<=0) up.pop_back();
        up.push_back(p);
    }
    lo.pop_back(); up.pop_back();
    lo.insert(lo.end(), up.begin(), up.end());
    return lo;
}
mt19937 rng(113);
int fail=0;
int main(){
    auto rand_convex=[&](int n, int B){
        vp v;
        for(int i=0;i<n+5;i++) v.push_back({(LD)((int)rng()%(2*B)-B),(LD)((int)rng()%(2*B)-B)});
        auto h=convex(v);
        if(h.empty()) h.push_back({0,0});
        return h;
    };
    auto same_set=[&](vp a, vp b){
        auto nrm=[&](vp w){ sort(w.begin(),w.end()); w.erase(unique(w.begin(),w.end(),[](P u,P v){return u.x==v.x&&u.y==v.y;}), w.end()); return w; };
        return nrm(a)==nrm(b);
    };
    for(int t=0;t<10000;t++){
        auto A=rand_convex(1+t%4, 6);
        auto B=rand_convex(1+t%4, 6);
        auto g=minkowski_sum(A,B);
        vp bf;
        for(auto u:A) for(auto v:B) bf.push_back(u+v);
        bf=convex(bf);
        auto gg=convex(g);
        if(!same_set(gg,bf) && gg.size()!=bf.size()){
            // compare as convex shapes: all vertices of one in hull of other
            auto inside=[&](vp h, P p){
                if(h.size()==1) return h[0].x==p.x&&h[0].y==p.y;
                if(h.size()==2) return turn(h[0],h[1],p)==0 && min(h[0].x,h[1].x)<=p.x && p.x<=max(h[0].x,h[1].x);
                for(size_t i=0;i<h.size();i++) if(turn(h[i], h[(i+1)%h.size()], p)<0) return false;
                return true;
            };
            bool ok=1;
            for(auto p:gg) if(!inside(bf,p)) ok=0;
            for(auto p:bf) if(!inside(gg,p)) ok=0;
            if(!ok){ fail++; if(fail<5) cerr<<"mink\n"; }
        }
    }
    int sf=fail;
    for(int t=0;t<1000;t++){
        auto A=rand_convex(5+t%6, 15);
        auto B=rand_convex(5+t%6, 15);
        auto g=convex(minkowski_sum(A,B));
        vp bf; for(auto u:A) for(auto v:B) bf.push_back(u+v);
        bf=convex(bf);
        auto inside=[&](vp h, P p){
            if(h.size()<=2) return true;
            for(size_t i=0;i<h.size();i++) if(turn(h[i], h[(i+1)%h.size()], p)<0) return false;
            return true;
        };
        bool ok=1;
        if(g.size()>=3 && bf.size()>=3){
            for(auto p:g) if(!inside(bf,p)) ok=0;
            for(auto p:bf) if(!inside(g,p)) ok=0;
        } else {
            auto nrm=[&](vp w){ sort(w.begin(),w.end()); return w; };
            ok = nrm(g).size()==nrm(bf).size();
        }
        if(!ok) fail++;
    }
    cout<<"dp_minkowski small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}