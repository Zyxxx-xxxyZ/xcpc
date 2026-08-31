from itertools import combinations, combinations_with_replacement, permutations, product


def RandomAndList():
	import random
	random.normalvariate(0.5, 0.1)
	l = [str(i) for i in range(9)]
	sorted(l), min(l), max(l), len(l)
	random.shuffle(l)
	l.sort(key=lambda x: ord(x) ^ 1, reverse=True)
	from functools import cmp_to_key
	l.sort(key=cmp_to_key(lambda x, y: (ord(y)^1)-(ord(x)^1)))
	for i in product('ABCD', repeat=2):
		pass # AA AB AC AD BA BB BC BD CA CB CC CD DA DB DC DD
	for i in permutations('ABCD', 2):
		pass # AB AC AD BA BC BD CA CB CD DA DB DC
	for i in combinations('ABCD', 2):
		pass # AB AC AD BC BD CD
	for i in combinations_with_replacement('ABCD', 2):
		pass # AA AB AC AD BB BC BD CC CD DD
def FractionOperation():
	from fractions import Fraction 
	a = Fraction(0.233).limit_denominator(1000)
	a.numerator, a.denominator, str(a)
def DecimalOperation():
	import decimal
	from decimal import Decimal, getcontext, FloatOperation
	getcontext().prec = 100
	getcontext().rounding = getattr(decimal, 'ROUND_HALF_EVEN')
	# default; other: FLOOR, CEILING, DOWN, ...
	getcontext().traps[FloatOperation] = True
	Decimal((0, (1, 4, 1, 4), -3)) # 1.414
	a = Decimal(1<<31) / Decimal(100000)
	print(f"{a:.9f}") # 21474.83648
	print(a.sqrt(), a.ln(), a.log10(), a.exp(), a ** 2)
def Complex():
	a = 1-2j
	print(a.real, a.imag, a.conjugate())
def FastIO():
	import sys, atexit, io
	_INPUT_LINES = sys.stdin.read().splitlines()
	input = iter(_INPUT_LINES).__next__
	_OUTPUT_BUFFER = io.StringIO()
	sys.stdout = _OUTPUT_BUFFER
	@atexit.register
	def write():
		sys.__stdout__.write(_OUTPUT_BUFFER.getvalue())
