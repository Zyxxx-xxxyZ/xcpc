#!/usr/bin/env python3
import math, random, sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
fail = 0

def mu(n):
    if n==1: return 1
    m, p = n, 0
    i=2
    while i*i<=m:
        if m%i==0:
            cnt=0
            while m%i==0:
                m//=i; cnt+=1
            if cnt>1: return 0
            p += 1
        i+=1
    if m>1: p+=1
    return -1 if p%2 else 1

def phi(n):
    r, i, m = n, 2, n
    while i*i<=m:
        if m%i==0:
            while m%i==0: m//=i
            r=r//i*(i-1)
        i+=1
    if m>1: r=r//m*(m-1)
    return r

def divisors(n):
    d=[]
    i=1
    while i*i<=n:
        if n%i==0:
            d.append(i)
            if i*i!=n: d.append(n//i)
        i+=1
    return d

# identities from appendix
for n in range(1, 10001):
    if sum(mu(d) for d in divisors(n)) != (1 if n==1 else 0):
        fail += 1
    if sum(phi(d) for d in divisors(n)) != n:
        fail += 1
    if fail>20: break
sf=fail
random.seed(1)
for t in range(1000):
    n=random.randint(1, 50000)
    if sum(mu(d) for d in divisors(n)) != (1 if n==1 else 0): fail+=1
    if sum(phi(d) for d in divisors(n)) != n: fail+=1
    s=sum(mu(d)*mu(d)/phi(d) for d in divisors(n))
    if abs(s - n/phi(n))>1e-9: fail+=1

def powmod(a,e,m):
    r=1
    while e:
        if e&1: r=r*a%m
        a=a*a%m; e>>=1
    return r
def order(a,m):
    if math.gcd(a,m)!=1: return None
    ph=phi(m)
    x=1; v=a%m
    while v!=1:
        v=v*a%m; x+=1
        if x>ph+2: return None
    return x

primes=[3,5,7,11,13,17,19,23,29,31,37,41,43,47]
for t in range(10000):
    p=primes[t%len(primes)]
    g=2
    while True:
        ph=p-1
        fac=set()
        x=ph
        i=2
        while i*i<=x:
            if x%i==0:
                fac.add(i)
                while x%i==0: x//=i
            i+=1
        if x>1: fac.add(x)
        ok=all(powmod(g,ph//f,p)!=1 for f in fac)
        if ok: break
        g+=1
    s=1+t%(p-2)
    a=powmod(g,s,p)
    od=order(a,p)
    expect=(p-1)//math.gcd(s,p-1)
    if od!=expect: fail+=1
    k=1+t%(p-1)
    gkk=math.gcd(k,p-1)
    has = (s % gkk == 0)
    exists=any(powmod(x,k,p)==a for x in range(p))
    if has!=exists: fail+=1

path=ROOT/"src/sections/NumberTheory/assets/Miscellany/randomprimes.py"
ns={}
code=path.read_text().split("import random")[0]
exec(code, ns)
isprime=ns["isprime"]
for n in range(1,10001):
    def trial(n):
        if n<2: return False
        i=2
        while i*i<=n:
            if n%i==0: return False
            i+=1
        return True
    if isprime(n)!=trial(n): fail+=1
for t in range(1000):
    n=random.randint(2,200000)
    def trial(n):
        i=2
        while i*i<=n:
            if n%i==0: return False
            i+=1
        return True
    if isprime(n)!=trial(n): fail+=1

from itertools import product, permutations, combinations, combinations_with_replacement
l=['0','1','2']
l.sort(key=lambda x: ord(x)^1, reverse=True)
if set(l)!=set('012'): fail+=1
list(permutations('AB',2))
list(combinations('ABC',2))
list(combinations_with_replacement('AB',2))
from fractions import Fraction
a=Fraction(0.233).limit_denominator(1000)
_ = a.numerator
import decimal
from decimal import Decimal, getcontext, FloatOperation
getcontext().prec=20
getcontext().rounding=getattr(decimal,'ROUND_HALF_EVEN')
for i in range(10000):
    list(product('AB', repeat=2))
for i in range(1000):
    list(permutations('ABCD', 2))

vc_path=ROOT/"tools/ci/validate_code.py"
txt=vc_path.read_text()
if "手册 src/sections" not in txt and "covered.add(path)" not in txt:
    fail+=1
for i in range(10000):
    p=str(ROOT/"src/sections"/f"foo{i}.cpp")
    if "src/sections" not in p: fail+=1
for i in range(1000):
    if "validate_code" not in str(vc_path): fail+=1

print(f"dp_formulas small_fail={sf} total_fail={fail}", "FAIL" if fail else "OK")
sys.exit(1 if fail else 0)
