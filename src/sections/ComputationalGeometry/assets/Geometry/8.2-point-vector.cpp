struct P {
	LD x, y;

	P(LD x_ = 0, LD y_ = 0) : x(x_), y(y_) {}
	P(cp a) : x(a.x), y(a.y) {}
	P &operator=(cp a) { x = a.x, y = a.y; return *this; }

	P rot(LD t) const { // 逆时针旋转 t 弧度
		LD c = cosl(t), s = sinl(t);
		return {x * c - y * s, x * s + y * c};
	}
	P rot90() const { return {-y, x}; }
	P _rot90() const { return {y, -x}; }
	LD len() const { return sqrtl(x * x + y * y); }
	LD len2() const { return x * x + y * y; }
	P unit() const { // 零向量的单位向量约定为零向量
		LD d = len();
		return sgn(d) ? P{x / d, y / d} : P{};
	}

	void read() { if (scanf("%Lf%Lf", &x, &y) != 2) x = y = 0; }
	void print() const { printf("(%.9Lf,%.9Lf)", x, y); }
};

bool operator<(cp a, cp b) { return a.x == b.x ? a.y < b.y : a.x < b.x; } // 序关系用精确比较, eps 只用于 == / 转向等谓词
bool operator>(cp a, cp b) { return b < a; }
bool operator==(cp a, cp b) { return !sgn(a.x - b.x) && !sgn(a.y - b.y); }
bool operator!=(cp a, cp b) { return !(a == b); }
P operator+(cp a, cp b) { return {a.x + b.x, a.y + b.y}; }
P operator-(cp a, cp b) { return {a.x - b.x, a.y - b.y}; }
P operator-(cp a) { return {-a.x, -a.y}; }
P operator*(cp a, LD k) { return {a.x * k, a.y * k}; }
P operator*(LD k, cp a) { return a * k; }
P operator/(cp a, LD k) { return {a.x / k, a.y / k}; }
LD operator*(cp a, cp b) { return a.x * b.x + a.y * b.y; } // 点积
LD operator^(cp a, cp b) { return a.x * b.y - a.y * b.x; } // 叉积

LD rad(cp a, cp b) { // 夹角 [0, pi]
	LD d = a.len() * b.len();
	if (!sgn(d)) return 0;
	return acosl(clamp((a * b) / d, -1.0L, 1.0L));
}
bool left(cp a, cp b) { return sgn(a ^ b) > 0; }
int turn(cp a, cp b, cp c) { return sgn((b - a) ^ (c - a)); }
int half(cp a) { return a.y > 0 || (a.y == 0 && a.x > 0); }
