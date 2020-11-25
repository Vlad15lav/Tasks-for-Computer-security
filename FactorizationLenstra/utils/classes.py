# Class for Elliptic Curve
class EllipticCurve:
    def __init__(self, a, b, n):
        self.a = a
        self.b = b
        self.n = n

# Class for Point
class Point:
    def __init__(self, X, Y, Z):
        self.X = X
        self.Y = Y
        self.Z = Z
    
    def set(self, point): # Set point copy
        self.X, self.Y, self.Z = point.getPoint()

    def setPoint(self, X, Y, Z): # Set point
        self.X = X
        self.Y = Y
        self.Z = Z
    
    def getPoint(self): # Get point coords
        return self.X, self.Y, self.Z

    def xy2xyz(self): # to projective
        self.Z = 1
    
    def xyz2xy(self): # to cartesian
        self.X /= self.Z
        self.Y /= self.Z

    def doubling(self, curve): # 2P
        X, Y, Z = self.getPoint()
        a = curve.a
        n = curve.n

        if Y == 0:
            self.setPoint(0, 1, 0)
            return

        if Z == 0: return
        
        A = a * pow(Z, 2, n) + 3 * pow(X, 2, n)
        B = Y * Z
        C = X * Y * B
        H = pow(A, 2, n) - 8 * C
        X2 = (2 * H * B) % n
        Y2 = (A * (4 * C - H) - 8 * pow(Y, 2, n) * pow(B, 2, n)) % n
        Z2 = pow(2 * B, 3, n)
        self.setPoint(X2, Y2, Z2)
    
    def add(self, point, curve): # P + Q
        X1, Y1, Z1 = self.getPoint()
        X2, Y2, Z2 = point.getPoint()

        if Z1 == 0: return self.setPoint(X2, Y2, Z2)
        if Z2 == 0: return
        
        a, b, n = curve.a, curve.b, curve.n
        
        A1 = Y2 * Z1 % n
        A2 = Y1 * Z2 % n
        B1 = X2 * Z1 % n
        B2 = X1 * Z2 % n

        if B1 == B2:
            if A1 == A2:
                self.doubling(curve)
            else:
                self.setPoint(0, 1, 0)
            return

        B = (B1 - B2) % n
        A = (A1 - A2) % n
        E = (Z1 * Z2) % n
        C = pow(A, 2, n) * E - pow(B, 3, n) - 2 * pow(B, 2, n) * B2
        X3 = (B * C) % n
        Y3 = (A * (pow(B, 2, n) * B2 - C) - pow(B, 3, n) * A2) % n
        Z3 = (pow(B, 3, n) * E) % n
        self.setPoint(X3, Y3, Z3)
    
    def kPmultiply(self, k, curve): # Find kP
        if k == 1:
            return
        P2 = Point(0, 1, 0)
        k2 = 0
        bit = 1 << (len(bin(k)) - 3)
        
        while k != k2:
            # 2P
            k2 = k2 << 1
            if k2: 
                P2.doubling(curve)
            # P + Q
            if k & bit:
                k2 += 1
                P2.add(self, curve)
            bit = bit >> 1
        self.set(P2)