#include <bits/stdc++.h>
using namespace std;
using LD = long double;
int sgn(LD x){ return fabsl(x)<1e-9L?0:(x<0?-1:1); }
struct P { LD x,y; P(LD x_=0,LD y_=0):x(x_),y(y_){} };
using cp = const P&;
bool operator==(cp a, cp b){ return !sgn(a.x-b.x)&&!sgn(a.y-b.y); }
P operator+(cp a,cp b){return {a.x+b.x,a.y+b.y};}
P operator-(cp a,cp b){return {a.x-b.x,a.y-b.y};}
P operator*(cp a,LD k){return {a.x*k,a.y*k};}
P operator/(cp a,LD k){return {a.x/k,a.y/k};}
LD operator*(cp a,cp b){return a.x*b.x+a.y*b.y;}
LD operator^(cp a,cp b){return a.x*b.y-a.y*b.x;}
struct L { P s,t; };
using cl = const L&;
bool point_on_line(cp a, cl l) { return !sgn((a - l.s) ^ (l.t - l.s)); }
bool point_on_segment(cp a, cl l) {
	return point_on_line(a, l) && sgn((a - l.s) * (a - l.t)) <= 0; }
bool point_on_ray(cp a, cl b) {
	if (b.s == b.t) return a == b.s;
	return point_on_line(a, b) && sgn((a - b.s) * (b.t - b.s)) >= 0; }
mt19937 rng(111);
int fail=0;
int main(){
    auto rnd=[&](){ return (LD)((int)rng()%21-10); };
    for(int t=0;t<10000;t++){
        L ray{{rnd(),rnd()},{rnd(),rnd()}};
        P a{rnd(),rnd()};
        bool g=point_on_ray(a,ray);
        bool bf;
        if(ray.s==ray.t) bf=(a==ray.s);
        else {
            P d=ray.t-ray.s, v=a-ray.s;
            bf = !sgn(v^d) && sgn(v*d)>=0;
        }
        if(g!=bf) fail++;
        // on-segment brute vs handbook
        bool gs=point_on_segment(a,ray);
        P d=ray.t-ray.s, v=a-ray.s;
        bool bs = (ray.s==ray.t)? (a==ray.s) : (!sgn(v^d) && sgn((a-ray.s)*(a-ray.t))<=0);
        if(gs!=bs) fail++;
    }
    int sf=fail;
    for(int t=0;t<1000;t++){
        // integer coords for exact
        L ray{{(LD)(rng()%20),(LD)(rng()%20)},{(LD)(rng()%20),(LD)(rng()%20)}};
        for(int k=0;k<10;k++){
            P a{(LD)(rng()%20),(LD)(rng()%20)};
            bool g=point_on_ray(a,ray);
            P d=ray.t-ray.s, v=a-ray.s;
            bool bf = (ray.s==ray.t)? (a==ray.s) : (!sgn(v^d)&&sgn(v*d)>=0);
            if(g!=bf) fail++;
        }
    }
    cout<<"dp_ray small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}