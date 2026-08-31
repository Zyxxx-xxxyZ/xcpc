poly poly_auto_mul(poly a, poly b) { // 自动判断长度的乘法
	int res_len = (int)a.size() + (int)b.size() - 1;
	int ntt_n = 1; while (ntt_n < res_len) ntt_n *= 2;
	a.resize(ntt_n); b.resize(ntt_n);
	ntt(a.data(), ntt_n, 1); ntt(b.data(), ntt_n, 1);
	for (int i = 0; i < ntt_n; i++)
		a[i] = (LL)a[i] * b[i] % p;
	ntt(a.data(), ntt_n, -1); a.resize(res_len); return a; }
// 多项式除法，a 和 b 长度可以任意
// 商的长度是 n - m + 1，余数的长度是 m - 1
poly poly_div(const poly& a, const poly& b) {
	poly bb = b; while (bb.size() > 1 && bb.back() == 0) bb.pop_back();
	int n = (int)a.size(), m = (int)bb.size();
	if (!m || bb.back() == 0) { assert(false); return {}; } // 零多项式
	if (n < m) return {};
	int ntt_n = 1; while (ntt_n < n - m + 1) ntt_n *= 2;
	poly f(ntt_n), g(ntt_n);
	for (int i = 0; i < n - m + 1; i++) f[i] = a[n - i - 1];
	for (int i = 0; i < m && i < n - m + 1; i++)
		g[i] = bb[m - i - 1];
	auto g_inv = poly_inv(g);
	fill(g_inv.begin() + n - m + 1, g_inv.end(), 0);
	auto c = poly_auto_mul(f, g_inv); c.resize(n - m + 1);
	reverse(c.begin(), c.end()); return c; }
// 多项式取模，a 和 b 长度可以任意，返回 (余数，商)
pair<poly, poly> poly_mod(const poly& a, const poly& b) {
	poly bb = b; while (bb.size() > 1 && bb.back() == 0) bb.pop_back();
	if (bb.empty() || bb.back() == 0) { assert(false); return {{}, {}}; } // 零多项式
	int n = (int)a.size(), m = (int)bb.size();
	if (n < m) { poly r = a; r.resize(max(0, m - 1)); return {r, {}}; }
	auto d = poly_div(a, bb); auto c = poly_auto_mul(bb, d);
	poly r(m - 1);
	for (int i = 0; i < m - 1; i++) {
		int ci = i < (int)c.size() ? c[i] : 0;
		int ai = i < (int)a.size() ? a[i] : 0;
		r[i] = (ai - ci + p) % p; }
	return {r, d}; }
