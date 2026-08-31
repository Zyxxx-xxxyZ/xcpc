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
P ll_intersection(cl a, cl b) { // 两直线需不平行
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
