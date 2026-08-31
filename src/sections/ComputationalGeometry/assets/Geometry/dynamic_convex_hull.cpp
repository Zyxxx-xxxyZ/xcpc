struct hull { // upper hull, left to right
set<P> a; LD tot;
hull () {tot = 0;}
template<class It> LD calc(It it) {
	auto u = it == a.begin() ? a.end() : prev(it);
	auto v = next(it);
	LD ret = 0;
	if (u != a.end()) ret += *u ^ *it;
	if (v != a.end()) ret += *it ^ *v;
	if (u != a.end() && v != a.end()) ret -= *u ^ *v;
	return ret; }
void insert (P p) {
	if (!a.size()) { a.insert (p); return; }
	auto it = a.lower_bound(p);
	if (it != a.end() && it->x == p.x) {
		if (p.y <= it->y) return;
		erase(it); it = a.lower_bound(p); }
	else if (it != a.begin() && prev(it)->x == p.x) {
		auto o = prev(it);
		if (p.y <= o->y) return;
		erase(o); it = a.lower_bound(p); }
	bool out = it == a.begin() || it == a.end()
		|| turn(*prev(it), *it, p) > 0;
	if (!out) return;
	while (a.size() >= 2 && it != a.begin()) {
		auto o = prev(it);
		if (o == a.begin()) break;
		if (turn(*prev(o), *o, p) < 0) break;
		erase(o); it = a.lower_bound(p); }
	while (it != a.end()) {
		auto o = next(it);
		if (o == a.end() || turn(p, *it, *o) < 0) break;
		else erase(it), it = o; }
	tot += calc(a.insert(p).first); }
template<class It> void erase(It it) { tot -= calc(it); a.erase(it); } };
