using poly = vector<int>;
poly poly_calc(const poly& u, const poly& v, // 长度要相同
	function<int(int, int)> op) { // 返回长度是两倍
	static int a[MAXN], b[MAXN], c[MAXN];
	int n = (int)u.size(); assert(u.size() == v.size());
	int N = 1; while (N < n) N *= 2;
	memcpy(a, u.data(), sizeof(int) * n); fill(a + n, a + N * 2, 0);
	memcpy(b, v.data(), sizeof(int) * n); fill(b + n, b + N * 2, 0);
	ntt(a, N * 2, 1); ntt(b, N * 2, 1);
	for (int i = 0; i < N * 2; i++) c[i] = op(a[i], b[i]);
	ntt(c, N * 2, -1); return poly(c, c + n * 2); }
poly poly_mul(const poly& u, const poly& v) { // 乘法
	return poly_calc(u, v, [](int a, int b)
		{ return (LL)a * b % p; }); } // 返回长度是两倍
poly poly_inv(const poly& a) { // 求逆，返回长度不变
	int n = (int)a.size(), N = 1; while (N < n) N *= 2;
	poly aa = a; aa.resize(N);
	poly c{qpow(aa[0], p - 2)}; // 常数项一般都是 1
	for (int k = 2; k <= N; k *= 2) {
		c.resize(k); poly b(aa.begin(), aa.begin() + k);
		c = poly_calc(b, c, [](int bi, int ci) {
			return ((2 - (LL)bi * ci) % p + p) * ci % p; });
		memset(c.data() + k, 0, sizeof(int) * k); }
	c.resize(n); return c; }
poly poly_sqrt(const poly& a) { // 开根，返回长度不变
	int n = (int)a.size(), N = 1; while (N < n) N *= 2;
	poly aa = a; aa.resize(N);
	poly c{1}; // 常数项不是 1 的话要写二次剩余
	for (int k = 2; k <= N; k *= 2) {
		c.resize(k); poly b(aa.begin(), aa.begin() + k);
		b = poly_mul(b, poly_inv(c));
		for (int i = 0; i < k; i++) // inv_2 是 2 的逆元
			c[i] = (LL)(c[i] + b[i]) * inv_2 % p; }
	c.resize(n); return c; }
poly poly_derivative(const poly& a) { poly c(a.size());
	for (int i = 1; i < (int)a.size(); i++) // 求导
		c[i - 1] = (LL)a[i] * i % p; return c; }
poly poly_integrate(const poly& a) { poly c(a.size());
	for (int i = 1; i < (int)a.size(); i++) // 不定积分
		c[i] = (LL)a[i - 1] * inv[i] % p; return c; }
poly poly_ln(const poly& a) { // ln(a/a[0])，常数项为 0；要 ln a 则 a[0]=1
	auto c = poly_mul(poly_derivative(a), poly_inv(a));
	c.resize(a.size()); return poly_integrate(c); }
// exp，常数项必须是 0，返回长度不变
// 常数很大并且总代码很长, 一般可以改用分治 FFT
// 依据: 设 $G(x) = \exp F(x)$, 则 $g_i = \frac{1}{i} \sum_{k=1}^{i-1} g_{i-k} \, k \, f_k$
poly poly_exp(const poly& a) {
	int n = (int)a.size(), N = 1; while (N < n) N *= 2;
	poly aa = a; aa.resize(N); poly c{1};
	for (int k = 2; k <= N; k *= 2) {
		c.resize(k); auto b = poly_ln(c);
		for (int i = 0; i < k; i++)
			b[i] = (aa[i] - b[i] + p) % p;
		(++b[0]) %= p; c = poly_mul(b, c);
		memset(c.data() + k, 0, sizeof(int) * k); }
	c.resize(n); return c; }
