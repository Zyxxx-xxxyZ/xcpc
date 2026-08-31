// $x^2 - n * y^2 = 1$ 最小正整数根, n 为完全平方数时无解
// $ x_{k+1} = x_0x_k + ny_0y_k $
// $ y_{k+1} = x_0y_k + y_0x_k $
pair<LL, LL> peLL(LL n) {
	static __int128 p[N], q[N], g[N], h[N], a[N];
	if (n <= 0) return {0, 0};
	p[1] = q[0] = h[1] = 1; p[0] = q[1] = g[1] = 0;
	__int128 s = (LL)floor(sqrtl((long double)n));
	while ((s + 1) * (s + 1) <= n) ++s;
	while (s * s > n) --s;
	if (s * s == n) return {0, 0}; // square: no solution
	a[2] = s;
	for(int i = 2; i < N - 2; i ++) {
		if (!h[i - 1]) return {0, 0};
		g[i] = -g[i - 1] + a[i] * h[i - 1];
		h[i] = (n - g[i] * g[i]) / h[i - 1];
		if (!h[i]) return {0, 0};
		a[i + 1] = (g[i] + a[2]) / h[i];
		p[i] = a[i] * p[i - 1] + p[i - 2];
		q[i] = a[i] * q[i - 1] + q[i - 2];
		if (p[i] < 0 || q[i] < 0 || p[i] > LLONG_MAX || q[i] > LLONG_MAX) return {0, 0};
		if(p[i] * p[i] - (__int128)n * q[i] * q[i] == 1)
			return {(LL)p[i], (LL)q[i]};
	}
	return {0, 0}; }