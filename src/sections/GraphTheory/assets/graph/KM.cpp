ll e[N][M]; //O(l^2*r)  最大权完备匹配; 要求 n<=m (不足则右侧补 0 列)
vector<int> KM(int n, int m) {
	if (n > m) m = n; // 右侧补虚点, e[][] 缺省为 0
	vector<ll> l(n + 1), r(m + 1);
	vector<int> p(m + 1), ans(n + 1);
	for (int i = 1; i <= n; ++i) {
		l[i] = e[i][1];
		for (int j = 2; j <= m; ++j) l[i] = max(l[i], e[i][j]); }
	for (int i = 1; i <= n; ++i) {
		vector<ll> d(m + 1, 1e18);
		vector<int> pre(m + 1), done(m + 1);
		int x, v = 0, u = 0;
		for (p[0] = i; x = p[u]; u = v) {
			done[u] = 1;
			ll mn = 1e18; v = 0;
			for (int j = 1; j <= m; ++j) if (!done[j]) {
				auto w = l[x] + r[j] - e[x][j];
				if (w < d[j]) d[j] = w, pre[j] = u;
				if (d[j] < mn) mn = d[j], v = j;
			}
			if (!v) break; // 无完备匹配
			for (int j = 0; j <= m; ++j) {
				if (done[j]) l[p[j]] -= mn, r[j] += mn;
				else d[j] -= mn;
			}
		}
		if (v) for (int vv; u; u = vv) vv = pre[u], p[u] = p[vv];
	}
	for (int j = 1; j <= m; ++j) if (p[j] && p[j] <= n) ans[p[j]] = j;
	return ans; }