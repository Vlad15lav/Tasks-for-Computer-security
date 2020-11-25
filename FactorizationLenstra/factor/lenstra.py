from utils.classes import *
from utils.tools import *
from factor.lenstra import *
import time
import math

# The algorithm is the factorization of Lenstra
def Lenstra(N, B, curves):
    start_time = time.time()
    for i in range(curves):
        g = N
        if g == N: # Random Curve and Point
            x, y, a = RandomCurve(N)
            b = (pow(y, 2, N) - pow(x, 3, N) - a * x) % N
            g = gcd(N, 4 * pow(a, 3) + 27 * pow(b, 2))
        
        if 1 < g < N: # Found g | N
            return {'Factorization': g, 'Curves': i + 1, 'X': x, 'Y': y, 'A': a, 'B': b, 'time': time.time() - start_time}

        P = Point(x, y, 1) 
        curve = EllipticCurve(a, b, N)

        M = 1 # Find M for MP point
        for p in range(2, B):
            if isPrime(p):
                M *= pow(p, int(math.log(B, p)))

        P.kPmultiply(M, curve) # Calculation MP
        gcd_nz = gcd(N, P.Z) # Check GCD for N and P.Z
        if gcd_nz > 1: # Found gcd_nz | N
            return {'Factorization': gcd_nz, 'Curves': i + 1, 'X': x, 'Y': y, 'A': a, 'B': b, 'time': time.time() - start_time}
    return None