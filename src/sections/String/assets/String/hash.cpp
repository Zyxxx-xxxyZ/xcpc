static constexpr u128 inv = []() {
	u128 ret = P;
	for (int i = 0; i < 6; i++) ret *= 2 - ret * P;
	return ret; }();
constexpr u128 chk = u128(-1) / P;
bool check(u128 a, u128 b) { // 无符号，避免 2^127 溢出
	if (a < b) swap(a, b);
	return (a - b) * inv <= chk; }