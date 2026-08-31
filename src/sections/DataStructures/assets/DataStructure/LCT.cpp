namespace LCT{
#define ls(x) c[x][0]
#define rs(x) c[x][1]
int val[N], ans[N], c[N][2], f[N], rev[N];
void push_up(int x){
	ans[x] = val[x];
	if (ls(x)) ans[x] = max(ans[x], ans[ls(x)]);
	if (rs(x)) ans[x] = max(ans[x], ans[rs(x)]); }
void opt(int x) { if (!x) return; swap(ls(x), rs(x)), rev[x] ^= 1;}
void push_down(int x) {
	if(rev[x]) { if (ls(x)) opt(ls(x)); if (rs(x)) opt(rs(x)); rev[x] = 0; }}
bool nrt(int x) {return x && (ls(f[x]) == x || rs(f[x]) == x);}
bool get(int x) {return x == rs(f[x]);}
void rot(int x){
	int y = f[x], z = f[y], t = get(x), w = c[x][t^1];
	if(nrt(y)) c[z][get(y)] = x;
	c[x][t^1] = y, c[y][t] = w;
	if(w) {f[w] = y;} f[y] = x, f[x] = z; push_up(y); }
void push_all(int x) {if(nrt(x)) push_all(f[x]); push_down(x);}
void splay(int x){
	push_all(x);
	while(nrt(x)){
		int y = f[x];
		if(nrt(y)) rot(get(x) == get(y) ? y : x);
		rot(x);
	} push_up(x); }
void access(int x) {for(int y=0;x;y=x, x=f[x]) splay(x), rs(x) = y, push_up(x);}
void mkrt(int x) {access(x); splay(x); opt(x);}
int findrt(int x){
	access(x), splay(x);
	while(ls(x)) push_down(x), x = ls(x);
	splay(x); return x; }
void split(int x, int y) {mkrt(x), access(y), splay(y);}
bool reach(int x, int y) {mkrt(x); return findrt(y) == x;}
void link(int x, int y) {if(!reach(x, y)) f[x] = y;}
void cut(int x, int y) {if(reach(x, y) && f[y] == x && !ls(y)) f[y] = rs(x) = 0;}
// 维护子树信息树的直径, F[x]:splay子树深度最小的点往下最大深度
// 直径版须自备结构操作, 全部走 opt_diam / push_up_diam, 不能复用上面的 splay/mkrt
void push_up_diam(int x){
	if (!x) return;
	siz[x] = siz[ls(x)] + siz[rs(x)] + 1;
	F[x] = max(F[ls(x)], F[rs(x)] + siz[ls(x)] + 1);
	G[x] = max(G[rs(x)], G[ls(x)] + siz[rs(x)] + 1);
	if(s[x].size()) 
		F[x] = max(F[x], *(--s[x].end()) + 1 + siz[ls(x)]),
		G[x] = max(G[x], *(--s[x].end()) + 1 + siz[rs(x)]); }
void opt_diam(int x) { if (!x) return; swap(ls(x), rs(x)), swap(F[x], G[x]), rev[x] ^= 1;}
void push_down_diam(int x) {
	if(rev[x]) { if (ls(x)) opt_diam(ls(x)); if (rs(x)) opt_diam(rs(x)); rev[x] = 0; }}
void rot_diam(int x){
	int y = f[x], z = f[y], t = get(x), w = c[x][t^1];
	if(nrt(y)) c[z][get(y)] = x;
	c[x][t^1] = y, c[y][t] = w;
	if(w) {f[w] = y;} f[y] = x, f[x] = z; push_up_diam(y); }
void push_all_diam(int x) {if(nrt(x)) push_all_diam(f[x]); push_down_diam(x);}
void splay_diam(int x){
	push_all_diam(x);
	while(nrt(x)){
		int y = f[x];
		if(nrt(y)) rot_diam(get(x) == get(y) ? y : x);
		rot_diam(x);
	} push_up_diam(x); }
void access_diam(int x) {
	for(int y=0;x;y=x, x=f[x]) {
		splay_diam(x);
		if(rs(x)) s[x].insert(F[rs(x)]);
		int oldF = y ? F[y] : 0;
		if(rs(x) = y) { auto it = s[x].find(oldF); if (it != s[x].end()) s[x].erase(it); }
		push_up_diam(x); } }
void mkrt_diam(int x) {access_diam(x); splay_diam(x); opt_diam(x);}
int findrt_diam(int x){
	access_diam(x), splay_diam(x);
	while(ls(x)) push_down_diam(x), x = ls(x);
	splay_diam(x); return x; }
void split_diam(int x, int y) {mkrt_diam(x), access_diam(y), splay_diam(y);}
bool reach_diam(int x, int y) {mkrt_diam(x); return findrt_diam(y) == x;}
void link_diam(int x, int y) {mkrt_diam(x);if(findrt_diam(y) != x) mkrt_diam(y), f[x] = y, s[y].insert(F[x]), push_up_diam(y);}
void cut_diam(int x, int y) {if(reach_diam(x, y) && f[y] == x && !ls(y)) f[y] = rs(x) = 0, push_up_diam(x);}
#undef ls
#undef rs
}
