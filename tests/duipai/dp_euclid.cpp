#include <bits/stdc++.h>
using namespace std;
using LL = long long;
using i128 = __int128;

struct Mat {
    LL a[2][2];
    Mat(LL w=1, LL x=0, LL y=0, LL z=1) { a[0][0]=w; a[0][1]=x; a[1][0]=y; a[1][1]=z; }
};
Mat operator*(const Mat& A, const Mat& B) {
    Mat C(0,0,0,0);
    for (int i=0;i<2;i++) for (int k=0;k<2;k++) for (int j=0;j<2;j++)
        C.a[i][j] += A.a[i][k] * B.a[k][j];
    return C;
}
bool operator==(const Mat& A, const Mat& B) {
    for (int i=0;i<2;i++) for (int j=0;j<2;j++) if (A.a[i][j] != B.a[i][j]) return false;
    return true;
}
Mat ksm(Mat v, LL k) {
    Mat r; // identity
    if (k < 0) { cerr << "neg exp\n"; abort(); }
    while (k) { if (k & 1) r = r * v; v = v * v; k >>= 1; }
    return r;
}
using Val = Mat;

Val work(LL P, LL R, LL Q, LL n, Val VU, Val VR) {
	if (Q <= 0 || n < 0 || R < 0) return VR;
	if (R >= Q) return work(P, R % Q, Q, n, VU, VR);
	if(!(((i128)n * P + R) / Q)) return ksm(VR, n);
	if(P>=Q) return work(P%Q,R,Q,n, VU, ksm(VU, P/Q) * VR);
	Val res; swap(VU,VR);
	res = ksm(VU, (Q-R-1)/P)*VR;
	LL m = ((i128)n * P + R) / Q;
	res = res * work(Q, (Q-R-1)%P, P, m-1, VU, VR);
	return res * ksm(VU, n - ((i128)m*Q - R - 1) / P); }

Val brute(LL P, LL R, LL Q, LL n, Val VU, Val VR) {
    Val ans; // identity
    LL py = 0; // start x=0, but R may be >=Q handled by work; brute assumes R>=0,Q>0
    // start y at floor(R/Q) but first step from x=0 to x=1 uses floor((P+R)/Q)-floor(R/Q)
    py = R / Q;
    for (LL x = 1; x <= n; x++) {
        LL y = (LL)(((i128)P * x + R) / Q);
        for (LL t = py; t < y; t++) ans = ans * VU;
        ans = ans * VR;
        py = y;
    }
    return ans;
}

mt19937_64 rng(3);
int fail = 0;
void oops(string s) { if (fail < 10) cerr << "FAIL " << s << "\n"; fail++; }

Val VU(1,1,0,1), VR(1,0,1,1);

void one(LL P, LL R, LL Q, LL n) {
    if (Q <= 0 || n < 0 || R < 0) return;
    Val a = work(P, R, Q, n, VU, VR);
    Val b = brute(P, R, Q, n, VU, VR);
    if (!(a == b)) {
        oops("P="+to_string(P)+" R="+to_string(R)+" Q="+to_string(Q)+" n="+to_string(n));
    }
}

int main() {
    for (int t = 0; t < 10000; t++) {
        LL Q = 1 + rng() % 30;
        LL P = rng() % 40;
        LL R = rng() % (Q * 3); // includes R>=Q
        LL n = rng() % 40;
        one(P, R, Q, n);
        if (fail > 15) break;
    }
    int small_fail = fail;
    for (int t = 0; t < 1000; t++) {
        LL Q = 1 + rng() % 200;
        LL P = rng() % 300;
        LL R = rng() % (Q * 4);
        LL n = 50 + rng() % 400;
        one(P, R, Q, n);
        if (fail > 25) break;
    }
    // extra: R>=Q reduction vs brute counts at larger n using identity-like count
    cout << "dp_euclid small_fail=" << small_fail << " total_fail=" << fail << (fail ? " FAIL\n" : " OK\n");
    return fail ? 1 : 0;
}
