import random

def gcd(a, b): # Find GCD
    while b:
        a, b = b, a % b
    return a

def RandomCurve(n): # Random X Y A for Curve
    return random.randint(0, n), random.randint(0, n), random.randint(0, n)
  
def MiillerRabin(d, n): # Test Miller Rabin on prime number
    a = 2 + random.randint(1, n - 4)
  
    x = pow(a, d, n)
  
    if (x == 1 or x == n - 1): 
        return True

    while (d != n - 1): 
        x = (x * x) % n
        d *= 2 
  
        if (x == 1): 
            return False
        if (x == n - 1): 
            return True
  
    return False

def isPrime(n): # Check on prime number
    if (n <= 1 or n == 4): 
        return False
    if (n <= 3): 
        return True
  
    t = n - 1; 
    while (t % 2 == 0): 
        t //= 2

    if (MiillerRabin(t, n) == False): 
        return False
  
    return True
