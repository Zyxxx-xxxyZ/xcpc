#include <bits/stdc++.h>
using namespace std;
int z[5005], n; string bstr; int b[5005];
void init(){
	int l = 0, r = 0;
	z[1] = n;
	for(int i=2;i<=n;i++){
		if(i <= r && z[i-l+1] < r-i+1) z[i] = z[i-l+1];
		else {
			z[i] = max(0, r-i+1);
			while(i+z[i]<=n && b[i+z[i]]==b[z[i]+1]) z[i]++;
		}
		if(i + z[i] - 1 > r) l = i, r = i + z[i] - 1;
	}
}
mt19937 rng(12);
int fail=0;
int main(){
    for(int t=0;t<10000;t++){
        n=1+t%25;
        for(int i=1;i<=n;i++) b[i]='a'+rng()%3;
        init();
        for(int i=1;i<=n;i++){
            int bf=0; while(i+bf<=n && b[i+bf]==b[1+bf]) bf++;
            if(z[i]!=bf){ fail++; break; }
        }
    }
    int sf=fail;
    for(int t=0;t<1000;t++){
        n=80+t%200;
        for(int i=1;i<=n;i++) b[i]='a'+rng()%4;
        init();
        for(int i=1;i<=n;i++){
            int bf=0; while(i+bf<=n && b[i+bf]==b[1+bf]) bf++;
            if(z[i]!=bf){ fail++; break; }
        }
    }
    cout<<"dp_exkmp small_fail="<<sf<<" total_fail="<<fail<<(fail?" FAIL\n":" OK\n");
    return fail?1:0;
}
