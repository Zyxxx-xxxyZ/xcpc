struct p3 {
	LD x, y, z;
	p3(LD x_ = 0, LD y_ = 0, LD z_ = 0) : x(x_), y(y_), z(z_) {}
	LD &operator[](int i) { return i == 0 ? x : (i == 1 ? y : z); }
	LD operator[](int i) const { return i == 0 ? x : (i == 1 ? y : z); }
	LD len2() const { return x*x + y*y + z*z; }
	LD len() const { return sqrtl(len2()); }
	p3 unit() const { LD d = len(); return {x/d, y/d, z/d}; }
	void read() { if (scanf("%Lf%Lf%Lf", &x, &y, &z) != 3) x = y = z = 0; }
};
p3 operator+(p3 a, p3 b) { return {a.x+b.x, a.y+b.y, a.z+b.z}; }
p3 operator-(p3 a, p3 b) { return {a.x-b.x, a.y-b.y, a.z-b.z}; }
p3 operator-(p3 a) { return {-a.x, -a.y, -a.z}; }
p3 operator*(p3 a, LD k) { return {a.x*k, a.y*k, a.z*k}; }
p3 operator*(LD k, p3 a) { return a*k; }
p3 operator/(p3 a, LD k) { return {a.x/k, a.y/k, a.z/k}; }
bool operator==(p3 a, p3 b) {
	return !sgn(a.x-b.x) && !sgn(a.y-b.y) && !sgn(a.z-b.z);
}
LD dot(p3 a, p3 b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
p3 cross(p3 a, p3 b) {
	return {a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x};
}
LD mix(p3 a, p3 b, p3 c) { return dot(cross(a, b), c); }

LD rotation[3][3]; // 右手系，绕单位向量 axis 旋转
void rotation_matrix(p3 axis, LD angle) {
	axis = axis.unit();
	LD c = cosl(angle), s = sinl(angle);
	for (int i = 0; i < 3; ++i) {
		int j = (i + 1) % 3, k = (i + 2) % 3;
		LD x = axis[i], y = axis[j], z = axis[k];
		rotation[i][i] = (y*y + z*z)*c + x*x;
		rotation[i][j] = x*y*(1-c) - z*s;
		rotation[i][k] = x*z*(1-c) + y*s;
	}
}

struct l3 { p3 s, t; };
struct plane {
	p3 normal; LD offset; // dot(normal, x) = offset，normal 为单位向量
	plane(p3 normal_ = {1,0,0}, p3 point = {})
		: normal(normal_.unit()), offset(dot(normal, point)) {}
};
p3 project_to_plane(p3 a, const plane &b) {
	return a + b.normal * (b.offset - dot(a, b.normal));
}
pair<p3, p3> closest_points(l3 x, l3 y) { // 两直线需异面且不平行
	p3 u = x.t-x.s, v = y.t-y.s, r = x.s-y.s;
	LD a = dot(u,u), b = dot(u,v), e = dot(v,v), d = a*e-b*b;
	LD c = dot(u,r), f = dot(v,r);
	LD s = (b*f-c*e)/d, t = (a*f-b*c)/d;
	return {x.s+u*s, y.s+v*t};
}
p3 line_plane_intersection(const plane &a, l3 b) {
	LD t = (a.offset-dot(a.normal,b.s))/dot(a.normal,b.t-b.s);
	return b.s+(b.t-b.s)*t;
}
l3 plane_plane_intersection(const plane &a, const plane &b) {
	p3 d = cross(a.normal,b.normal);
	LD d2 = d.len2();
	p3 s = cross(b.normal*a.offset-a.normal*b.offset,d)/d2;
	return {s,s+d};
}
p3 three_plane_intersection(const plane &a, const plane &b, const plane &c) {
	LD d = mix(a.normal,b.normal,c.normal);
	return (cross(b.normal,c.normal)*a.offset
		+ cross(c.normal,a.normal)*b.offset
		+ cross(a.normal,b.normal)*c.offset)/d;
}
