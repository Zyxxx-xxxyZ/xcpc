LL modmul(LL a, LL b, LL M) { // M in (0, 2^63)
	LL ret = (LL)((__int128)a * b % M);
	return ret < 0 ? ret + M : ret; }
ULL modmul(ULL a, ULL b, LL M) {
	ULL m = (ULL)M;
	return (ULL)((unsigned __int128)a * b % m); }
struct DIV { 
	ULL p, ip;
	void init (ULL _p) { p = _p; ip = -1llu / p; }
	ULL mod (ULL x) { // x < 2 ^ 64, residue in [0, p)
		ULL q = ULL(((u128)ip * x) >> 64);
		ULL r = x - q * p;
		return r >= p ? r - p : r;
} }; // speedup only when mod is not const