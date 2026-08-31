vector <int> E[N];
vector <int> ml, mr, a, p;
void match (int nl, int nr) { // 1-based
	ml.assign(nl + 1, 0);
	mr.assign(nr + 1, 0); // nr
	while (true) {
		bool ok = 0;
		a.assign(nl + 1, 0);
		p.assign(nl + 1, 0); // nl
		static queue <int> q;
		for (int i = 1; i <= nl; i++)
			if (!ml[i]) a[i] = p[i] = i, q.push(i);
		while (!q.empty()) {
			int x = q.front(); q.pop();
			if (ml[a[x]]) continue;
			for (auto y : E[x]) {
				if (!mr[y]) {
					for (ok = 1; y; x = p[x])
						mr[y] = x, swap(ml[x], y);
					break;
				} else if (!p[mr[y]])
					q.push(y = mr[y]), p[y] = x, a[y] = a[x];
			} } // while (!q.empty())
		if (!ok) break; } }
array<vector<int>, 2> min_vertex_cover(int nl, int nr) { // Konig
 match(nl, nr); vector <int> l, r;
 for (int i = 1; i <= nl; i++) if (!a[i]) l.push_back(i);
 for (int i = 1; i <= nr; i++) if (a[mr[i]]) r.push_back(i);
 return {l, r}; }
vector<pair<int,int>> min_edge_cover(int nl, int nr) { // |V|-matching
 match(nl, nr); vector<pair<int,int>> es;
 vector<int> uL(nl+1), uR(nr+1);
 for (int i = 1; i <= nl; i++) if (ml[i])
  es.push_back({i, ml[i]}), uL[i] = uR[ml[i]] = 1;
 for (int i = 1; i <= nl; i++) if (!uL[i])
  for (int y : E[i]) { es.push_back({i, y}); uL[i] = uR[y] = 1; break; }
 for (int j = 1; j <= nr; j++) if (!uR[j])
  for (int i = 1; i <= nl && !uR[j]; i++)
   for (int y : E[i]) if (y == j) { es.push_back({i, j}); uR[j] = 1; break; }
 return es; }