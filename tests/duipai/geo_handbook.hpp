#pragma once
// 手册活页 ComputationalGeometry 8.1–8.4 及后续片段的测试副本（不 #include 手册路径）。
#include <bits/stdc++.h>
using namespace std;

#define cp const P &
#define cl const L &
#define cc const C &
#define vp vector<P>
#define cvp const vector<P> &
#define D double
#define LD long double
using LL = long long;

mt19937 rnd(20260831);
const LD eps = 1e-12L;
const LD pi = acosl(-1.0L);
const LD INF = 1e18L;

int sgn(LD x) {
	return x > eps ? 1 : (x < -eps ? -1 : 0);
}

struct P {
	LD x, y;

	P(LD x_ = 0, LD y_ = 0) : x(x_), y(y_) {}
	P(cp a) : x(a.x), y(a.y) {}
	P &operator=(cp a) { x = a.x, y = a.y; return *this; }

	P rot(LD t) const {
		LD c = cosl(t), s = sinl(t);
		return {x * c - y * s, x * s + y * c};
	}
	P rot90() const { return {-y, x}; }
	P _rot90() const { return {y, -x}; }
	LD len() const { return sqrtl(x * x + y * y); }
	LD len2() const { return x * x + y * y; }
	P unit() const {
		LD d = len();
		return sgn(d) ? P{x / d, y / d} : P{};
	}

	void read() { if (scanf("%Lf%Lf", &x, &y) != 2) x = y = 0; }
	void print() const { printf("(%.9Lf,%.9Lf)", x, y); }
};

bool operator<(cp a, cp b) { return a.x == b.x ? a.y < b.y : a.x < b.x; }
bool operator>(cp a, cp b) { return b < a; }
bool operator==(cp a, cp b) { return !sgn(a.x - b.x) && !sgn(a.y - b.y); }
bool operator!=(cp a, cp b) { return !(a == b); }
P operator+(cp a, cp b) { return {a.x + b.x, a.y + b.y}; }
P operator-(cp a, cp b) { return {a.x - b.x, a.y - b.y}; }
P operator-(cp a) { return {-a.x, -a.y}; }
P operator*(cp a, LD k) { return {a.x * k, a.y * k}; }
P operator*(LD k, cp a) { return a * k; }
P operator/(cp a, LD k) { return {a.x / k, a.y / k}; }
LD operator*(cp a, cp b) { return a.x * b.x + a.y * b.y; }
LD operator^(cp a, cp b) { return a.x * b.y - a.y * b.x; }

LD rad(cp a, cp b) {
	LD d = a.len() * b.len();
	if (!sgn(d)) return 0;
	return acosl(clamp((a * b) / d, -1.0L, 1.0L));
}
bool left(cp a, cp b) { return sgn(a ^ b) > 0; }
int turn(cp a, cp b, cp c) { return sgn((b - a) ^ (c - a)); }
int half(cp a) { return a.y > 0 || (a.y == 0 && a.x > 0); }

struct L {
	P s, t;

	L(cp s_ = P(), cp t_ = P()) : s(s_), t(t_) {}
	L(cl a) : s(a.s), t(a.t) {}
	L &operator=(cl a) { s = a.s, t = a.t; return *this; }
	void read() { s.read(), t.read(); }
};

bool point_on_line(cp a, cl l) {
	return !sgn((a - l.s) ^ (l.t - l.s));
}
bool point_on_segment(cp a, cl l) {
	return point_on_line(a, l) && sgn((a - l.s) * (a - l.t)) <= 0;
}
bool two_side(cp a, cp b, cl l) {
	return sgn((a - l.s) ^ (l.t - l.s))
		* sgn((b - l.s) ^ (l.t - l.s)) < 0;
}
bool intersection_judge_strict(cl a, cl b) {
	return two_side(a.s, a.t, b) && two_side(b.s, b.t, a);
}
bool intersection_judge(cl a, cl b) {
	return point_on_segment(a.s, b) || point_on_segment(a.t, b)
		|| point_on_segment(b.s, a) || point_on_segment(b.t, a)
		|| intersection_judge_strict(a, b);
}
P ll_intersection(cl a, cl b) {
	LD s1 = (a.t - a.s) ^ (b.s - a.s);
	LD s2 = (a.t - a.s) ^ (b.t - a.s);
	return (b.s * s2 - b.t * s1) / (s2 - s1);
}
bool point_on_ray(cp a, cl b) {
	if (b.s == b.t) return a == b.s;
	return point_on_line(a, b) && sgn((a - b.s) * (b.t - b.s)) >= 0;
}
bool ray_intersection_judge(cl a, cl b) {
	if (!sgn((a.t - a.s) ^ (b.t - b.s))) {
		if (!point_on_line(b.s, a)) return false;
		return point_on_ray(b.s, a) || point_on_ray(a.s, b);
	}
	P p = ll_intersection(a, b);
	return point_on_ray(p, a) && point_on_ray(p, b);
}
LD point_to_line(cp a, cl b) {
	if (b.s == b.t) return (a - b.s).len();
	return fabsl((b.t - b.s) ^ (a - b.s)) / (b.t - b.s).len();
}
P project_to_line(cp a, cl b) {
	P d = b.t - b.s;
	if (b.s == b.t) return b.s;
	return b.s + d * ((a - b.s) * d / d.len2());
}
LD point_to_segment(cp a, cl b) {
	if (b.s == b.t) return (a - b.s).len();
	if (sgn((a - b.s) * (b.t - b.s)) >= 0
		&& sgn((a - b.t) * (b.s - b.t)) >= 0)
		return point_to_line(a, b);
	return min((a - b.s).len(), (a - b.t).len());
}
LD segment_to_segment(cl a, cl b) {
	if (intersection_judge(a, b)) return 0;
	return min({point_to_segment(a.s, b), point_to_segment(a.t, b),
		point_to_segment(b.s, a), point_to_segment(b.t, a)});
}

struct C {
	P c;
	LD r;

	C(cp c_ = P(), LD r_ = 0) : c(c_), r(r_) {}
	C(cc a) : c(a.c), r(a.r) {}
	C &operator=(cc a) { c = a.c, r = a.r; return *this; }
	C(cp a, cp b) : c((a + b) / 2), r((a - c).len()) {}
	C(cp x, cp y, cp z) {
		P p(y - x), q(z - x);
		P s(p.len2() / 2, q.len2() / 2);
		LD d = p ^ q;
		c = x + P(s ^ P(p.y, q.y), P(p.x, q.x) ^ s) / d;
		r = (c - x).len();
	}
};

bool in_circle(cp a, cc b) { return sgn((b.c - a).len() - b.r) <= 0; }

C min_circle(vp p) {
	if (p.empty()) return C();
	shuffle(p.begin(), p.end(), rnd);
	C ret(p[0], 0);
	for (int i = 1; i < (int)p.size(); ++i) if (!in_circle(p[i], ret)) {
		ret = C(p[i], 0);
		for (int j = 0; j < i; ++j) if (!in_circle(p[j], ret)) {
			ret = C(p[i], p[j]);
			for (int k = 0; k < j; ++k) if (!in_circle(p[k], ret))
				ret = C(p[i], p[j], p[k]);
		}
	}
	return ret;
}

vp lc_intersection(cl l, cc c) {
	LD d = point_to_line(c.c, l);
	if (sgn(d - c.r) > 0) return {};
	P p = project_to_line(c.c, l);
	LD x = sqrtl(max(0.0L, c.r * c.r - d * d));
	if (!sgn(x)) return {p};
	P v = (l.t - l.s).unit() * x;
	return {p - v, p + v};
}

LD cc_intersection_area(cc a, cc b) {
	LD d = (a.c - b.c).len();
	if (sgn(d - a.r - b.r) >= 0) return 0;
	if (sgn(d - fabsl(a.r - b.r)) <= 0) {
		LD r = min(a.r, b.r);
		return pi * r * r;
	}
	LD x = (d * d + a.r * a.r - b.r * b.r) / (2 * d);
	LD t1 = acosl(clamp(x / a.r, -1.0L, 1.0L));
	LD t2 = acosl(clamp((d - x) / b.r, -1.0L, 1.0L));
	return a.r * a.r * t1 + b.r * b.r * t2 - d * a.r * sinl(t1);
}

int cc_intersection_count(cc a, cc b) {
	LD d = (a.c - b.c).len();
	if (a.c == b.c) {
		if (sgn(a.r - b.r)) return 0;
		return !sgn(a.r) ? 1 : -1;
	}
	if (sgn(d - a.r - b.r) > 0
		|| sgn(d - fabsl(a.r - b.r)) < 0) return 0;
	if (!sgn(d - a.r - b.r) || !sgn(d - fabsl(a.r - b.r))) return 1;
	return 2;
}

vp cc_intersection(cc a, cc b) {
	int cnt = cc_intersection_count(a, b);
	if (cnt <= 0) return {};
	if (a.c == b.c) return {a.c};
	LD d = (a.c - b.c).len();
	P v = (b.c - a.c).unit();
	LD x = (a.r * a.r - b.r * b.r + d * d) / (2 * d);
	LD h = sqrtl(max(0.0L, a.r * a.r - x * x));
	P p = a.c + v * x;
	if (cnt == 1) return {p};
	return {p + v.rot90() * h, p - v.rot90() * h};
}

vp tangent(cp a, cc b) {
	return cc_intersection(b, C(a, b.c));
}

vector<L> tangent(cc a, cc b, int f) {
	P d = b.c - a.c;
	LD dr = b.r - f * a.r, d2 = d.len2();
	if (!sgn(d2) || sgn(d2 - dr * dr) < 0) return {};
	LD h = sqrtl(max(0.0L, d2 - dr * dr));
	vector<L> ret;
	for (int s : {-1, 1}) {
		P v = (d * dr + d.rot90() * (h * s)) / d2;
		ret.push_back({a.c + v * a.r, b.c + v * (f * b.r)});
		if (!sgn(h)) break;
	}
	return ret;
}

vp convex_hull (vp a) {
	sort (a.begin(), a.end());
	a.erase(unique(a.begin(), a.end()), a.end());
	int n = (int) a.size (), cnt = 0;
	vp ret;
	for (int i = 0; i < n; ++i) {
		while (cnt > 1
		&& turn (ret[cnt - 2], ret[cnt - 1], a[i]) <= 0)
			--cnt, ret.pop_back ();
		++cnt, ret.push_back (a[i]); }
	for (int i = n - 2, fixed = cnt; i >= 0; --i) {
		while (cnt > fixed
		&& turn (ret[cnt - 2], ret[cnt - 1], a[i]) <= 0)
			--cnt, ret.pop_back ();
		++cnt, ret.push_back (a[i]); }
	if (n > 1) ret.pop_back ();
	return ret; }

vp minkowski_sum(vp a, vp b) {
	auto yx = [](cp u, cp v) { return u.y != v.y ? u.y < v.y : u.x < v.x; };
	if (!a.empty()) rotate(a.begin(), min_element(a.begin(), a.end(), yx), a.end());
	if (!b.empty()) rotate(b.begin(), min_element(b.begin(), b.end(), yx), b.end());
	if (a.size() == 1 || b.size() == 1) {
		vp ret;
		for (auto i : a) for (auto j : b) ret.push_back(i+j);
		return ret; }
	vp x, y;
	for (size_t i = 0; i < a.size(); ++i)
		x.push_back(a[(i + 1) % a.size()] - a[i]);
	for (size_t i = 0; i < b.size(); ++i)
		y.push_back(b[(i + 1) % b.size()] - b[i]);
	vp ret (x.size() + y.size());
	merge(x.begin(), x.end(), y.begin(), y.end(),
		  ret.begin(), [](cp u, cp v) {
		return half(u) != half(v) ? half(u) > half(v) : (u ^ v) > 0;});
	P cur = a[0] + b[0];
	for (auto &i : ret) swap(i, cur), cur = cur + i;
	return ret; }

LD convex_diameter(cvp p) {
	int n = (int)p.size();
	if (n < 2) return 0;
	if (n == 2) return (p[0] - p[1]).len();
	LD ans2 = 0;
	for (int i = 0, j = 1; i < n; ++i) {
		int ni = (i + 1) % n;
		while (fabsl((p[ni] - p[i]) ^ (p[(j + 1) % n] - p[i]))
			> fabsl((p[ni] - p[i]) ^ (p[j] - p[i])) + eps)
			j = (j + 1) % n;
		ans2 = max({ans2, (p[i] - p[j]).len2(), (p[ni] - p[j]).len2()});
	}
	return sqrtl(ans2);
}

vp remove_repeated_convex_vertices(cvp p) {
	vp q;
	for (cp x : p) if (q.empty() || x != q.back()) q.push_back(x);
	if (q.size() > 1 && q.front() == q.back()) q.pop_back();
	return q;
}

LD convex_min_width(cvp input) {
	vp p = remove_repeated_convex_vertices(input);
	int n = (int)p.size();
	if (n < 3) return 0;
	LD ans = INF;
	for (int i = 0, j = 1; i < n; ++i) {
		int ni = (i + 1) % n;
		while (((p[ni] - p[i]) ^ (p[(j + 1) % n] - p[i]))
			> ((p[ni] - p[i]) ^ (p[j] - p[i])) + eps)
			j = (j + 1) % n;
		ans = min(ans, ((p[ni] - p[i]) ^ (p[j] - p[i]))
			/ (p[ni] - p[i]).len());
	}
	return ans;
}

struct MinRectangle {
	LD area = 0;
	array<P, 4> p{};
};

MinRectangle min_area_rectangle(cvp input) {
	vp p = remove_repeated_convex_vertices(input);
	int n = (int)p.size();
	if (!n) return {};
	if (n == 1) return {0, {p[0], p[0], p[0], p[0]}};
	MinRectangle ans{INF, {}};
	P first_u = (p[1] - p[0]).unit(), first_v = first_u.rot90();
	int high = 0, right = 0, left = 0;
	for (int j = 1; j < n; ++j) {
		if (p[j] * first_v > p[high] * first_v) high = j;
		if (p[j] * first_u > p[right] * first_u) right = j;
		if (p[j] * first_u < p[left] * first_u) left = j;
	}
	for (int i = 0; i < n; ++i) {
		P u = (p[(i + 1) % n] - p[i]).unit(), v = u.rot90();
		while (p[(high + 1) % n] * v > p[high] * v + eps)
			high = (high + 1) % n;
		while (p[(right + 1) % n] * u > p[right] * u + eps)
			right = (right + 1) % n;
		while (p[(left + 1) % n] * u < p[left] * u - eps)
			left = (left + 1) % n;
		LD min_u = p[left] * u, max_u = p[right] * u;
		LD min_v = p[i] * v, max_v = p[high] * v;
		LD area = (max_u - min_u) * (max_v - min_v);
		if (area < ans.area) ans = {area, {
			u * min_u + v * min_v, u * max_u + v * min_v,
			u * max_u + v * max_v, u * min_u + v * max_v}};
	}
	return ans;
}

vp polygon_cut(cvp p, cl l) {
	vp ret;
	int n = (int)p.size();
	if (!n) return ret;
	for (int i = 0; i < n; ++i) {
		P a = p[i], b = p[(i + 1) % n];
		int sa = turn(l.s, l.t, a), sb = turn(l.s, l.t, b);
		if (sa >= 0) ret.push_back(a);
		if (sa * sb < 0) ret.push_back(ll_intersection(l, {a, b}));
	}
	return ret;
}

LD polygon_signed_area2(cvp p) {
	LD ret = 0;
	for (int i = 0, n = (int)p.size(); i < n; ++i)
		ret += p[i] ^ p[(i + 1) % n];
	return ret;
}

LD polygon_area(cvp p) {
	return fabsl(polygon_signed_area2(p)) / 2;
}

LD polygon_perimeter(cvp p) {
	LD ret = 0;
	for (int i = 0, n = (int)p.size(); i < n; ++i)
		ret += (p[i] - p[(i + 1) % n]).len();
	return ret;
}

P polygon_centroid(cvp p) {
	LD area2 = polygon_signed_area2(p);
	if (!sgn(area2)) {
		P ret;
		LD perimeter = 0;
		for (int i = 0, n = (int)p.size(); i < n; ++i) {
			LD length = (p[i] - p[(i + 1) % n]).len();
			ret = ret + (p[i] + p[(i + 1) % n]) * length;
			perimeter += length;
		}
		if (sgn(perimeter)) return ret / (2 * perimeter);
		return p.empty() ? P{} : p[0];
	}
	P ret;
	for (int i = 0, n = (int)p.size(); i < n; ++i) {
		LD cross = p[i] ^ p[(i + 1) % n];
		ret = ret + (p[i] + p[(i + 1) % n]) * cross;
	}
	return ret / (3 * area2);
}

int point_in_polygon(cp u, cvp p) {
	bool in = false;
	for (int i = 0, n = (int)p.size(); i < n; ++i) {
		P a = p[i], b = p[(i + 1) % n];
		if (point_on_segment(u, {a, b})) return 0;
		if ((a.y > u.y) != (b.y > u.y)
			&& sgn((b - a) ^ (u - a)) == (b.y > a.y ? 1 : -1)) in = !in;
	}
	return in ? 1 : -1;
}

bool turn_left(cl a, cl b, cl c) {
  return turn(a.s, a.t, ll_intersection(b, c)) > 0; }
bool is_para(cl a, cl b){return !sgn((a.t-a.s) ^ (b.t-b.s));}
bool cmp(cl a, cl b) {
  int sign = half(a.t - a.s) - half(b.t - b.s);
  int dir = sgn((a.t - a.s) ^ (b.t - b.s));
  if (!dir && !sign) return turn(a.s, a.t, b.t) < 0;
  else return sign ? sign > 0 : dir > 0; }
vp hpi(vector<L> h) {
  sort(h.begin(), h.end(), cmp);
  vector<L> q(h.size()); int l = 0, r = -1;
  for(auto &i : h) {
   while (l < r && !turn_left(i, q[r - 1], q[r])) --r;
   while (l < r && !turn_left(i, q[l], q[l + 1])) ++l;
   if (l <= r && is_para(i, q[r])) continue;
   q[++r] = i; }
  while (r - l > 1 && !turn_left(q[l], q[r - 1], q[r])) --r;
  while (r - l > 1 && !turn_left(q[r], q[l], q[l + 1])) ++l;
  if(r - l < 2) return {};
  vp ret(r - l + 1);
  for(int i = l; i <= r; i++)
	ret[i - l] = ll_intersection(q[i], q[i == r ? l : i + 1]);
  return ret; }

struct IL : P {
	LD z;
	IL(LD a = 0, LD b = 0, LD c = 0) : P(a, b), z(c) {}
	IL(cp a, cp b) : P((b - a).rot90()), z(a ^ b) {}
	LD operator()(cp a) const { return a * static_cast<const P &>(*this) + z; }
};
using cil = const IL &;

P il_intersection(cil u, cil v) {
	return P(P(u.z, u.y) ^ P(v.z, v.y),
		P(u.x, u.z) ^ P(v.x, v.z)) / -(static_cast<const P &>(u)
		^ static_cast<const P &>(v));
}
LD signed_distance(cil l, cp x = P()) { return l(x) / l.len(); }
bool il_parallel(cil a, cil b) { return !sgn(a ^ b); }
LD det3(cil a, cil b, cil c) {
	return (a ^ b) * c.z + (b ^ c) * a.z + (c ^ a) * b.z;
}
int check(cil a, cil b, cil c) {
	return sgn(det3(b, c, a)) * sgn(b ^ c);
}
bool il_turn_left(cil a, cil b, cil c) { return check(a, b, c) > 0; }
bool il_cmp(cil a, cil b) {
	if (il_parallel(a, b) && a * b > 0)
		return signed_distance(a) < signed_distance(b);
	return half(a) == half(b) ? sgn(a ^ b) > 0 : half(b) > 0;
}

IL perpendicular(cil l) { return {l.y, -l.x, 0}; }
IL parallel_through(cil l, cp o) { return {l.x, l.y, l.z - l(o)}; }
P project(cp x, cil l) { return x - static_cast<const P &>(l) * (l(x) / l.len2()); }
P reflect(cp x, cil l) { return x - static_cast<const P &>(l) * (2 * l(x) / l.len2()); }
bool il_perpendicular(cil a, cil b) { return !sgn(a * b); }
LD triangle_area(cil a, cil b, cil c) {
	LD d = det3(a, b, c);
	return d * d / (2 * (a ^ b) * (b ^ c) * (c ^ a));
}

vector<IL> cut_integral_hpi(const vector<IL> &o, IL l) {
	vector<IL> ret;
	int n = (int)o.size();
	for (int i = 0; i < n; ++i) {
		cil u = o[i], v = o[(i + 1) % n], w = o[(i + 2) % n];
		int va = check(l, u, v), vb = check(l, v, w);
		if (va > 0 || vb > 0 || (!va && !vb)) ret.push_back(v);
		if (va >= 0 && vb < 0) ret.push_back(l);
	}
	return ret;
}

P incenter(cp a, cp b, cp c) {
	LD p = (a-b).len() + (b-c).len() + (c-a).len();
	return (a*(b-c).len() + b*(c-a).len() + c*(a-b).len()) / p; }
P circumcenter(cp a, cp b, cp c) { return C(a, b, c).c; }
P orthocenter(cp a, cp b, cp c) {
	return a + b + c - circumcenter(a, b, c) * 2; }
P fermat_point(cp a, cp b, cp c) {
	if (a == b) return a;
	if (b == c) return b;
	if (c == a) return c;
	LD ab = (a-b).len(), bc = (b-c).len(), ca = (c-a).len();
	LD cosa = ((b-a)*(c-a)) / ab / ca;
	LD cosb = ((a-b)*(c-b)) / ab / bc;
	LD cosc = ((b-c)*(a-c)) / ca / bc;
	LD sq3 = pi / 3; P mid;
	if (sgn (cosa + 0.5) < 0) mid = a;
	else if (sgn (cosb + 0.5) < 0) mid = b;
	else if (sgn (cosc + 0.5) < 0) mid = c;
	else if (sgn((b-a)^(c-a)) < 0)
		mid = ll_intersection({a, b+(c-b).rot(sq3)}, {b, c+(a-c).rot(sq3)});
	else mid = ll_intersection({a, c+(b-c).rot(sq3)}, {c, b+(a-b).rot(sq3)});
	return mid; }

C inv_c2c(P O, LD R, C A) {
	LD OA = (A.c - O).len();
	LD RB = 0.5 * R * R * (1 / (OA - A.r) - 1 / (OA + A.r));
	LD OB = OA * RB / A.r;
	P B = O + (A.c - O) * (OB / OA);
	return {B, RB};
}
C inv_l2c(P O, LD R, L l) {
	P p = project_to_line(O, l);
	LD d = (O - p).len();
	LD RB = R * R / (2 * d);
	P VB = (p - O) / d * RB;
	return {O + VB, RB};
}
L inv_c2l(P O, LD R, C A) {
	LD t = R * R / (2 * A.r);
	P p = O + (A.c - O).unit() * t;
	return {p, p + (O - p).rot90()};
}

LD sphereDis(LD lon1, LD lat1, LD lon2, LD lat2, LD R) {
	LD d = cosl(lat1) * cosl(lat2) * cosl(lon1 - lon2) + sinl(lat1) * sinl(lat2);
	return R * acosl(clamp(d, -1.0L, 1.0L)); }

LD closest_pair(vp p) {
	int n = (int)p.size();
	if (n < 2) return INF;
	sort(p.begin(), p.end());
	vector<P> tmp(n);
	function<LD(int, int)> solve = [&](int l, int r) -> LD {
		if (r - l <= 3) {
			LD ret = INF;
			for (int i = l; i < r; ++i)
				for (int j = i + 1; j < r; ++j)
					ret = min(ret, (p[i] - p[j]).len2());
			sort(p.begin() + l, p.begin() + r,
				[](cp a, cp b) { return a.y == b.y ? a.x < b.x : a.y < b.y; });
			return ret;
		}
		int m = (l + r) / 2;
		LD mid_x = p[m].x;
		LD ret = min(solve(l, m), solve(m, r));
		merge(p.begin() + l, p.begin() + m, p.begin() + m, p.begin() + r,
			tmp.begin(), [](cp a, cp b) {
				return a.y == b.y ? a.x < b.x : a.y < b.y;
			});
		copy(tmp.begin(), tmp.begin() + r - l, p.begin() + l);
		vector<P> strip;
		for (int i = l; i < r; ++i) if ((p[i].x - mid_x) * (p[i].x - mid_x) < ret) {
			for (int j = (int)strip.size() - 1; j >= 0
				&& (p[i].y - strip[j].y) * (p[i].y - strip[j].y) < ret; --j)
				ret = min(ret, (p[i] - strip[j]).len2());
			strip.push_back(p[i]);
		}
		return ret;
	};
	return sqrtl(solve(0, n));
}

vector <LL> lattice_circle_y(LL r) {
	vector <LL> ret;
	ret.push_back(0);
	LL l = 2 * r, s = sqrt(l);
	for (LL d=1; d<=s; d++) if (l%d==0) {
		LL lim=LL(sqrt(l/(2*d)));
		for (LL a = 1; a <= lim; a++) {
			LL b = sqrt(l/d-a*a);
			if (a*a+b*b==l/d && __gcd(a,b)==1 && a!=b)
				ret.push_back(d*a*b);
		} if (d*d==l) break;
		lim = sqrt(d/2);
		for (LL a=1; a<=lim; a++) {
			LL b = sqrt(d - a * a);
			if (a*a+b*b==d && __gcd(a,b)==1 && a!=b)
				ret.push_back(l/d*a*b);
	} } ret.push_back(r); return ret; }

LD angle_poly_circle (cp u, cp v) {
	return atan2l(fabsl(u ^ v), u * v); }
LD circle_edge_area2(cp s, cp t, LD r) {
	if (!sgn(s ^ t)) return 0;
	LD theta = angle_poly_circle(s, t);
	LD d = point_to_segment({0, 0}, {s, t});
	if (sgn(d - r) >= 0) return theta * r * r;
	auto q = lc_intersection({s, t}, C({0, 0}, r));
	if (q.size() < 2) return theta * r * r;
	P lo = sgn(s ^ q[0]) >= 0 ? q[0] : s;
	P hi = sgn(q[1] ^ t) >= 0 ? q[1] : t;
	return (lo ^ hi) + (theta - angle_poly_circle(lo, hi)) * r * r; }
LD polygon_circle_intersection_area(cvp p, cc c) {
	LD ret = 0;
	for (int i = 0; i < (int) p.size (); ++i) {
		auto u = p[i] - c.c;
		auto v = p[(i + 1) % p.size()] - c.c;
		int s = sgn(u ^ v);
		if	  (s > 0) ret += circle_edge_area2(u, v, c.r);
		else if (s < 0) ret -= circle_edge_area2(v, u, c.r);
	} return fabsl(ret) / 2; }

struct simpson {
LD area (LD (*f) (LD), LD l, LD r) {
	LD m = l + (r - l) / 2;
	return (f (l) + 4 * f (m) + f (r)) * (r - l) / 6;
}
LD solve (LD (*f) (LD), LD l, LD r, LD eps, LD a) {
	LD m = l + (r - l) / 2;
	LD left = area (f, l, m), right = area (f, m, r);
	if (abs (left + right - a) <= 15 * eps)
		return left + right + (left + right - a) / 15.0;
	return solve (f, l, m, eps / 2, left) + solve (f, m, r, eps / 2, right);
}
LD solve (LD (*f) (LD), LD l, LD r, LD eps) {
	return solve (f, l, r, eps, area (f, l, r));
}};
