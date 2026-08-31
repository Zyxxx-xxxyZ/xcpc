LL exgcd(LL a, LL b, LL &x, LL &y) {
	if (b == 0) return x = 1, y = 0, a;
	LL t = exgcd(b, a % b, y, x);
	y -= a / b * x; return t;}
LL inv(LL x, LL m) {
	LL a, b; LL g = exgcd(x, m, a, b);
	assert(g == 1 || g == -1);
	return (a % m + m) % m; }
