vp minkowski_sum(vp a, vp b) {
// size > 0, rotate to min (y, x), 无重
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
	return ret; } // ret 可能共线，但没有重点
