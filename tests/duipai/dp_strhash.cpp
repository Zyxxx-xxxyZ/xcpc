#include <bits/stdc++.h>
using namespace std;
using u128 = unsigned __int128;
constexpr u128 P = (u128)1000000007 * 1000000009; // odd
static constexpr u128 inv = []() {
	u128 ret = P;
	for (int i = 0; i < 6; i++) ret *= 2 - ret * P;
	return ret; }();
constexpr u128 chk = u128(-1) / P;
bool check(u128 a, u128 b) {
	if (a < b) swap(a, b);
	return (a - b) * inv <= chk; }
mt19937_64 rng(103);
int fail=0;
u128 rnd128(){ return ((u128)rng()<<64) ^ rng(); }
int main(){
    for(int t=0;t<10000;t++){
        u128 a=rnd128(), b;
        if(t%3==0) b=a;
        else if(t%3==1) b=a+P;
        else b=rnd128();
        bool g=check(a,b);
        bool bf=(a%P)==(b%P);
        if(g!=bf){ fail++; if(fail<8) cerr<<"hash t="<<t<<"\n"; }
    }
    int sf=fail;
    for(int t=0;t<1000;t++){
        u128 a=rnd128();
        u128 b=a + (u128)(t+1)*P;
        if(check(a,b) != 1) fail++;
        b=a + (u128)(t+1)*P + 1;
        if(check(a,b) != ((a%P)==(b%P))) fail++;
        u128 a2 = ((u128)1<<127) + (u128)rng();
        u128 b2 = a2 + P;
        if(check(a2,b2) != ((a2%P)==(b2%P))) fail++;
    }
    cout<<"dp_strhash small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}
