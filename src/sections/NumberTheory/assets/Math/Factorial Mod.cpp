// n! mod p^q Time : $O(pq^2\frac{\log^2n}{\log p})$
// Output : {a, b} means a*p^b
using Val=unsigned long long; //Val 需要 mod p^q 意义下 + *
typedef vector<Val> poly;
poly polymul(const poly &a,const poly &b, Val mod){
	int n = (int) a.size(); poly c (n, Val(0));
	for (int i = 0; i < n; ++ i) {
		for (int j = 0; i + j < n; ++ j) {
			c[i + j] = (Val)((unsigned __int128)(c[i + j] + (unsigned __int128)a[i] * b[j]) % mod); } }
	return c; } Val choo[70][70];
poly polyshift(const poly &a, Val delta, Val mod) {
	int n = (int) a.size(); poly res (n, Val(0));
	for (int i = 0; i < n; ++ i) { Val d = 1;
		for (int j = 0; j <= i; ++ j) {
			res[i - j] = (Val)((unsigned __int128)(res[i - j] + (unsigned __int128)a[i]*choo[i][j]%mod*d) % mod);
			d = (Val)((unsigned __int128)d * delta % mod); } } return res; }
void prepare(int q) {
	for (int i = 0; i < q; ++ i) { choo[i][0] = Val(1);
		for (int j = 1; j <= i; ++ j)
			choo[i][j]=choo[i-1][j-1]+choo[i-1][j]; } }
pair<Val, LL> fact(LL n, LL p, LL q) { prepare((int)q);
	Val mod = 1; for (int i = 0; i < q; ++i) mod *= (Val)p; Val ans = 1;
	for (int r = 1; r < p; ++ r) {
		poly x (q, Val(0)), res (q, Val(0));
		res[0] = 1; LL _res = 0; x[0] = r; LL _x = 0;
		if (q > 1) x[1] = p, _x = 1; LL m = (n - r + p) / p;
		while (m) { if (m & 1) {
				res=polymul(res,polyshift(x,_res,mod),mod); _res+=_x; }
			m >>= 1; x = polymul(x, polyshift(x, _x, mod), mod); _x+=_x; }
		ans = (Val)((unsigned __int128)ans * res[0] % mod); }
	LL cnt = n / p; if (n >= p) { auto tmp=fact(n / p, p, q);
		ans = (Val)((unsigned __int128)ans * tmp.first % mod); cnt += tmp.second; }
	return {ans, cnt}; }