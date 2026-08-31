#include <bits/stdc++.h>
using namespace std;
using LL = long long;
LL exgcd(LL a, LL b, LL &x, LL &y) {
	if (b == 0) return x = 1, y = 0, a;
	LL t = exgcd(b, a % b, y, x);
	y -= a / b * x; return t;}
LL gcd(LL a, LL b) { return b ? gcd(b, a % b) : a; }

LL mul_mod(LL x, LL y, LL m) { return (LL)((__int128)x * y % m); }
LL inv(LL a,LL n){LL x,y;exgcd(a,n,x,y);return (x % n + n) % n;}
LL bsgs(LL a,LL b,LL n){
	if (n <= 1) return n == 1 ? 0 : -1;
	b = (b % n + n) % n; a = (a % n + n) % n;
	LL m=(LL)sqrt((long double)n)+1,e=1,i;map<LL,LL>mp;mp[1]=0;
	for(i=1;i<m;i++)if(!mp.count(e=mul_mod(e,a,n)))mp[e]=i;
	e=mul_mod(e,a,n);e=inv(e,n);
	for(i=0;i<m;b=mul_mod(b,e,n),i++)if(mp.count(b))return i*m+mp[b];
	return -1;
}
LL exbsgs(LL a,LL b,LL n){
	if (n <= 1) return n == 1 ? 0 : -1;
	b=(b%n+n)%n; a=(a%n+n)%n;
	LL oa=a,ob=b,on=n,V,k=0,d,e=1;
	auto chk=[&](LL x){ LL r=1,p=oa,t=x;
		for(;t;t>>=1,p=mul_mod(p,p,on)) if(t&1) r=mul_mod(r,p,on); return r==ob; };
	if(ob==1) return 0;
	for(;(d=gcd(a,n))!=1;){
		if(b%d){ if(b==1 && chk(k)) return k; return -1; }
		k++;n=n/d;b=b/d;e=mul_mod(e,a/d,n);
		if(e==b && chk(k)) return k; }
	V=bsgs(a,mul_mod(b,inv(e,n),n),n); if(!~V) return -1;
	return chk(V+k)?V+k:-1;}

LL powmod(LL a, LL x, LL n) {
    LL r = 1 % n; a %= n; if (a < 0) a += n;
    while (x) { if (x & 1) r = mul_mod(r, a, n); a = mul_mod(a, a, n); x >>= 1; }
    return r;
}
LL brute_dlog(LL a, LL b, LL n) {
    a = (a % n + n) % n; b = (b % n + n) % n;
    if (n == 1) return 0;
    LL e = 1 % n;
    for (LL x = 0; x <= n; x++) {
        if (e == b) return x;
        e = mul_mod(e, a, n);
        if (x && e == (1 % n)) break; // cycle, if not found
    }
    // full scan if gcd!=1
    e = 1 % n;
    for (LL x = 0; x < n + 5; x++) {
        if (e == b) return x;
        e = mul_mod(e, a, n);
    }
    return -1;
}

mt19937_64 rng(4);
int fail = 0;
void oops(string s) { if (fail < 12) cerr << "FAIL " << s << "\n"; fail++; }

int main() {
    // small 10000: true brute
    for (int t = 0; t < 10000; t++) {
        LL n = 2 + rng() % 220;
        LL a = rng() % n;
        LL b = rng() % n;
        LL got = exbsgs(a, b, n);
        LL bf = brute_dlog(a, b, n);
        if (got != bf) {
            // brute may stop at order; if got>=0 verify pow
            bool ok = false;
            if (got >= 0 && powmod(a, got, n) == ((b % n + n) % n)) {
                if (bf < 0 || got == bf) ok = true;
                if (bf >= 0 && got > bf) { oops("not minimal n="+to_string(n)); }
                else ok = true;
            }
            if (got < 0 && bf < 0) ok = true;
            if (got < 0 && bf >= 0) oops("missed sol n="+to_string(n)+" a="+to_string(a)+" b="+to_string(b)+" x="+to_string(bf));
            if (got >= 0 && bf < 0) {
                if (powmod(a, got, n) != (b%n+n)%n) oops("false pos");
                else { /* brute incomplete */ }
            }
            if (got >= 0 && bf >= 0 && got != bf) {
                if (powmod(a, got, n) != (b%n+n)%n) oops("wrong val");
                else if (got > bf) oops("not min");
            }
        }
        if (fail > 20) break;
    }
    int small_fail = fail;
    // large 1000: n~ few thousand brute
    for (int t = 0; t < 1000; t++) {
        LL n = 500 + rng() % 2500;
        LL a = 1 + rng() % (n - 1);
        LL x = rng() % (n);
        LL b = powmod(a, x, n);
        LL got = exbsgs(a, b, n);
        if (got < 0 || powmod(a, got, n) != b) oops("large miss");
        else {
            // minimality: brute up to got
            LL e = 1 % n, minx = -1;
            for (LL i = 0; i <= got; i++) {
                if (e == b) { minx = i; break; }
                e = mul_mod(e, a, n);
            }
            if (minx != got) oops("large not min got="+to_string(got)+" min="+to_string(minx));
        }
        if (fail > 30) break;
    }
    int mid_fail = fail;
    // overflow domain: n > 2^32, verify by pow not brute scan
    int ovf_fail = 0;
    for (int t = 0; t < 1000; t++) {
        LL n = (1LL << 32) + (rng() % (1LL << 20)) * 2 + 1; // odd ~4e9
        LL a = 2 + rng() % 100000;
        if (gcd(a, n) != 1) { t--; continue; }
        LL x = rng() % 5000 + 1;
        LL b = powmod(a, x, n);
        LL got = bsgs(a, b, n);
        if (got < 0 || powmod(a, got, n) != b) { ovf_fail++; if (ovf_fail < 5) cerr << "ovf fail n="<<n<<" a="<<a<<" x="<<x<<" got="<<got<<"\n"; }
        else if (got > x) { /* not necessarily min if order | x-got, but should be min */ }
        if (got >= 0) {
            // check minimality among 0..got
            bool smaller = false;
            LL e = 1;
            for (LL i = 0; i < got; i++) {
                if (e == b) { smaller = true; break; }
                e = mul_mod(e, a, n);
                if (i > 6000) break; // got should be <=x+order; x<5000 so got should be small if min
            }
            if (smaller) { ovf_fail++; }
        }
    }
    fail += ovf_fail;
    cout << "dp_bsgs small_fail=" << small_fail << " after_large=" << mid_fail << " ovf_fail=" << ovf_fail
         << " total_fail=" << fail << (fail ? " FAIL\n" : " OK\n");
    return fail ? 1 : 0;
}
