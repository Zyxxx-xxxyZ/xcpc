// $\sum_{i=0}^{n-1} \lfloor \frac{a+bi}{m}\rfloor$, $n,m,a,b>0$
LL solve(LL n, LL a, LL b, LL m){
	assert(n >= 0 && m > 0 && a >= 0 && b >= 0);
	if (b == 0) return (LL)((__int128)n * (a / m));
	if (a >= m) return (LL)((__int128)n * (a / m) + solve(n, a % m, b, m));
	if (b >= m) return (LL)((__int128)(n-1)*n/2*(b/m) + solve(n,a,b%m,m));
	return solve((LL)(((__int128)a + (__int128)b * n) / m),
		(LL)(((__int128)a + (__int128)b * n) % m), m, b); }
