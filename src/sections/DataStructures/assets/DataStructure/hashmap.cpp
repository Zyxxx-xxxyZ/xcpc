template <class T,int P = 314159/*,451411,1141109,2119969*/>
struct hashmap {
ULL id[P]; T val[P];
int R[P + 1]; // del: few clears; R[0] 为占用数
hashmap() {memset(id, -1, sizeof id); memset(val, 0, sizeof val); R[0] = 0;}
T get(const ULL &x) const {
	for (int i = int(x % P), j = 1, steps = 0; ~id[i]; i = (i + j) % P, j = (j + 2) % P) {
		if (id[i] == x) return val[i];
		if (++steps >= P) break; }
	return 0; }
T& operator [] (const ULL &x) {
	for (int i = int(x % P), j = 1, steps = 0; ; i = (i + j) % P, j = (j + 2) % P) {
		if (id[i] == x) return val[i];
		else if (id[i] == -1llu) {
			id[i] = x; val[i] = T{};
			R[++R[0]] = i;  // del: few clears
			return val[i]; }
		if (++steps >= P) { assert(false); static T dummy{}; return dummy; } } } // 表满: 不写其它 key 的槽
void clear() { // del: few clears
for (int &x = R[0]; x; id[R[x]] = -1, val[R[x]] = 0, --x); }
void fullclear() {memset(id, -1, sizeof id); memset(val, 0, sizeof val); R[0] = 0; } };
