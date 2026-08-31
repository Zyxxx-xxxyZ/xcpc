#include <bits/stdc++.h>
using namespace std;
vector<int> GospersHack(int k, int n) {
    vector<int> all;
    if (k <= 0 || k > n || n >= 31) return all;
    for (int s = (1 << k) - 1, c, r; s < (1 << n);
        c = s & -s, r = s + c, s = (((r ^ s) >> 2) / c) | r)
        all.push_back(s);
    return all;
}
mt19937 rng(102);
int fail=0;
int main(){
    for(int t=0;t<10000;t++){
        int n=1+t%12, k=t%(n+1);
        auto got=GospersHack(k,n);
        vector<int> bf;
        if(k>=1 && k<=n && n<31){
            for(int s=0;s<(1<<n);s++) if(__builtin_popcount(s)==k) bf.push_back(s);
        }
        sort(got.begin(), got.end());
        sort(bf.begin(), bf.end());
        if(got!=bf){ fail++; if(fail<5) cerr<<"n="<<n<<" k="<<k<<"\n"; }
    }
    int sf=fail;
    for(int t=0;t<1000;t++){
        int n=12+t%10; // 12..21
        int k=1+t%(n);
        auto got=GospersHack(k,n);
        vector<int> bf;
        for(int s=0;s<(1<<n);s++) if(__builtin_popcount(s)==k) bf.push_back(s);
        sort(got.begin(), got.end());
        sort(bf.begin(), bf.end());
        if(got!=bf) fail++;
    }
    // n>=31 must not infinite / UB: just return empty
    if(!GospersHack(3,31).empty()) fail++;
    cout<<"dp_gosper small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}
