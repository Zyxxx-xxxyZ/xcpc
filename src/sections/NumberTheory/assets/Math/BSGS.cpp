LL mul_mod(LL x, LL y, LL m) { return (LL)((__int128)x * y % m); }
LL inv(LL a,LL n){LL x,y;exgcd(a,n,x,y);return (x % n + n) % n;}
LL bsgs(LL a,LL b,LL n){// 在 (a,n)=1 时求最小的 x 使得 a^x mod n=b 
	if (n <= 1) return n == 1 ? 0 : -1;
	b = (b % n + n) % n; a = (a % n + n) % n;
	LL m=(LL)sqrt((long double)n)+1,e=1,i;map<LL,LL>mp;mp[1]=0;
	for(i=1;i<m;i++)if(!mp.count(e=mul_mod(e,a,n)))mp[e]=i;
	e=mul_mod(e,a,n);e=inv(e,n);// e=a^m, 求出其逆元后放到等式右边 
	for(i=0;i<m;b=mul_mod(b,e,n),i++)if(mp.count(b))return i*m+mp[b];
	return -1;// 无解 
}LL exbsgs(LL a,LL b,LL n){// 求最小的 x 使 a^x mod n=b 
	if (n <= 1) return n == 1 ? 0 : -1;
	b=(b%n+n)%n; a=(a%n+n)%n;
	LL oa=a,ob=b,on=n,V,k=0,d,e=1;
	auto chk=[&](LL x){ LL r=1,p=oa,t=x;
		for(;t;t>>=1,p=mul_mod(p,p,on)) if(t&1) r=mul_mod(r,p,on); return r==ob; };
	if(ob==1) return 0;
	for(;(d=gcd(a,n))!=1;){
		if(b%d){ if(b==1 && chk(k)) return k; return -1; }
		k++;n=n/d;b=b/d;e=mul_mod(e,a/d,n);
		if(e==b && chk(k)) return k; }
	V=bsgs(a,mul_mod(b,inv(e,n),n),n); if(!~V) return -1;
	return chk(V+k)?V+k:-1;}// 有解返回 V+k
