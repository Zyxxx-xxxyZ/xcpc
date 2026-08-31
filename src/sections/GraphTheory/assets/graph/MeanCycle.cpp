// 点标号为$1, 2, \cdots, n$. f[k][v] : 恰好 k 条边走到 v 的最短路
ll f[N][N]; int u[M], v[M], w[M];
for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) f[i][j] = Inf;
for (int i = 1; i <= n; i++) f[0][i] = 0;
for(int i = 1; i <= n; i ++)
	for(int j = 0; j < m; j ++)
		f[i][v[j]] = min(f[i][v[j]], f[i - 1][u[j]] + w[j]);
double ans = Inf;
for(int i = 1; i <= n; i ++) if (f[n][i] < Inf / 2) {
	double t = -Inf; bool ok = 0;
	for(int j = 0; j < n; j ++) if (f[j][i] < Inf / 2) {
		t = max(t, (f[n][i] - f[j][i]) / (double)(n - j)); ok = 1; }
	if (ok) ans = min(t, ans); }