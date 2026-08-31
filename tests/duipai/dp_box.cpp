#include <bits/stdc++.h>
using namespace std;
using ll = long long;
ll r;
void go(int i, int j, ll x, ll y, ll z,ll x0, ll y0, ll L, ll W, ll H) {
if (z==0) { ll R = x*x+y*y; if (R<r) r=R;}
else {
if(i>=0&&i<2)go(i+1,j, x0+L+z, y, x0+L-x, x0+L, y0, H,W,L);
if(j>=0&&j<2)go(i,j+1, x, y0+W+z, y0+W-y, x0, y0+W, L,H,W);
if(i<=0&&i>-2)go(i-1, j, x0-z, y, x-x0, x0-H, y0, H, W, L);
if(j<=0&&j>-2)go(i, j-1, x, y0-z, y-y0, x0, y0-H, L, H, W);
} }
ll algo(ll L, ll W, ll H, ll x1, ll y1, ll z1, ll x2, ll y2, ll z2){
    if (z1!=0 && z1!=H) if (y1==0 || y1==W)
        swap(y1,z1), swap(y2,z2), swap(W,H);
    else swap(x1,z1), swap(x2,z2), swap(L,H);
    if (z1==H) z1=0, z2=H-z2;
    r=0x3fffffffffffffffLL;
    go(0,0,x2-x1,y2-y1,z2,-x1,-y1,L,W,H);
    return r;
}
// independent unfolding: p1 forced to z=0 by same rotation as algo, then min over common nets
ll brute_after_norm(ll L,ll W,ll H, ll x1,ll y1, ll x2,ll y2,ll z2){
    auto sq=[&](ll a,ll b){return a*a+b*b;};
    ll ans=LLONG_MAX;
    auto ck=[&](ll a,ll b){ ans=min(ans,sq(a,b)); };
    if(z2==0) ck(x2-x1,y2-y1);
    // unfold p2 onto plane of z=0 by rolling
    ck(x2-x1, y2 + z2);           // roll over y=0 or y=W side? this is +z into y
    ck(x2-x1, y2 - z2);
    ck(x2 + z2, y2-y1);
    ck(x2 - z2, y2-y1);
    ck(x2-x1, (W-y1)+(W-y2)+z2); // around far y
    ck(x2-x1, y1+y2+z2);
    ck((L-x1)+(L-x2)+z2, y2-y1);
    ck(x1+x2+z2, y2-y1);
    // top face via four sides
    ck(x2-x1, y2 + (H-z2) + W-y1); // messy
    // 6 classic:
    ck(x1+x2, y1-y2);
    ck(x1-x2, y1+y2);
    ck(x1+z2, y1-y2);
    ck(x1-x2, y1+z2);
    return ans;
}
int fail=0;
mt19937 rng(119);
int main(){
    auto on_surface=[&](ll L,ll W,ll H,ll x,ll y,ll z){
        if(x<0||x>L||y<0||y>W||z<0||z>H) return false;
        return x==0||x==L||y==0||y==W||z==0||z==H;
    };
    for(int t=0;t<10000;t++){
        ll L=1+t%6, W=1+t%6, H=1+t%6;
        ll x1=rng()%(L+1), y1=rng()%(W+1), z1=rng()%(H+1);
        ll x2=rng()%(L+1), y2=rng()%(W+1), z2=rng()%(H+1);
        if(!on_surface(L,W,H,x1,y1,z1) || !on_surface(L,W,H,x2,y2,z2)) { t--; continue; }
        ll g=algo(L,W,H,x1,y1,z1,x2,y2,z2);
        // geodesic cannot be shorter than 3D chord, cannot be longer than path via edges
        ll chord=(x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2);
        if(g<chord) { fail++; if(fail<5) cerr<<"shorter than chord\n"; }
        if(g<0) fail++;
        // if same point
        if(x1==x2&&y1==y2&&z1==z2 && g!=0) fail++;
    }
    int sf=fail;
    for(int t=0;t<1000;t++){
        ll L=5+t%10, W=5+t%10, H=5+t%10;
        ll x1=0, y1=rng()%(W+1), z1=rng()%(H+1); // on x=0 face
        ll x2=L, y2=rng()%(W+1), z2=rng()%(H+1); // opposite
        ll g=algo(L,W,H,x1,y1,z1,x2,y2,z2);
        ll chord=L*L+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2);
        if(g<chord) fail++;
        // path via a side should be achievable: e.g. unfold to (L, dy) + z
        ll via = L*L + (y1-y2)*(y1-y2); // through interior not allowed; surface via top: 
        // at least one unfolding: (x2-x1)^2+(y2-y1 + H? )
        if(g > (L+W)*(L+W) + (ll)H*H * 4) fail++; // loose upper
    }
    cout<<"dp_box small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}