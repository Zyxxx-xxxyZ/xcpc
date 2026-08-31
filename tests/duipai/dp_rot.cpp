#include <bits/stdc++.h>
using namespace std;
using LD = long double;
int sgn(LD x){ return fabsl(x)<1e-9L?0:(x<0?-1:1); }
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
p3 operator*(p3 a, LD k) { return {a.x*k, a.y*k, a.z*k}; }
LD dot(p3 a, p3 b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
p3 cross(p3 a, p3 b) {
	return {a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x};
}
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
p3 apply(p3 v){
    p3 r;
    for(int i=0;i<3;i++) r[i]=rotation[i][0]*v[0]+rotation[i][1]*v[1]+rotation[i][2]*v[2];
    return r;
}
p3 rodrigues(p3 k, p3 v, LD ang){
    k=k.unit();
    LD c=cosl(ang), s=sinl(ang);
    return v*c + cross(k,v)*s + k*dot(k,v)*(1-c);
}
mt19937 rng(24);
LD rnd(){ return (LD)((int)(rng()%2001)-1000)/10; }
int fail=0;
int main(){
    for(int t=0;t<10000;t++){
        p3 k{rnd(),rnd(),rnd()};
        if(k.len()<1e-6) k={1,0,0};
        LD ang = (LD)((int)(rng()%628)-314)/100;
        p3 v{rnd(),rnd(),rnd()};
        rotation_matrix(k, ang);
        p3 a=apply(v), b=rodrigues(k,v,ang);
        if(fabsl(a.x-b.x)+fabsl(a.y-b.y)+fabsl(a.z-b.z) > 1e-6) fail++;
        // preserve length
        if(fabsl(a.len()-v.len())>1e-6) fail++;
    }
    int sf=fail;
    for(int t=0;t<1000;t++){
        p3 k{rnd(),rnd(),rnd()}; if(k.len()<1e-6) k={0,1,0};
        LD ang = (LD)((int)(rng()%6283)-3141)/1000;
        p3 v{rnd(),rnd(),rnd()};
        rotation_matrix(k, ang);
        p3 a=apply(v), b=rodrigues(k,v,ang);
        if(fabsl(a.x-b.x)+fabsl(a.y-b.y)+fabsl(a.z-b.z) > 1e-5) fail++;
    }
    cout<<"dp_rot small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}
