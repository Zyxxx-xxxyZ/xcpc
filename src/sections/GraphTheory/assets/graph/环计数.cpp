// Time complexity: $O(m\sqrt{m})$
for(int i=1;i<=m;i++) {
	g[x[i]].push_back(y[i]); g[y[i]].push_back(x[i]);
	if(deg[x[i]] > deg[y[i]] || (deg[x[i]] == deg[y[i]] && x[i] > y[i])) swap(x[i], y[i]);
	e[x[i]].push_back(y[i]);}
// 四元环计数: 第一跳走定向边, 第二跳走无向邻域, 终点严格在 i 之上
for(int i=1;i<=n;i++) {
	for(auto x : e[i]) for(auto y : g[x])
		if(y != i && (deg[i] < deg[y] || (deg[i] == deg[y] && i < y)))
			ans += cnt[y]++;
	for(auto x : e[i]) for(auto y : g[x]) cnt[y] = 0;}