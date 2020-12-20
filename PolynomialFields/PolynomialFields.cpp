#include <iostream>
#include <queue>
#include <stack>

using namespace std;

int* DecToBin(int); // Converting decimal number to binary
void NumberIrreducible(int, int); // Number of irreducible polynomials of degree d(F)
void NumberPrimitive(int, int); // Number of primitive polynomials of degree d(F)

class Polynomial
{
private:
	int *p; // Coefficients of the polynomial (from free to high)
	int degree; // Degree of the polynomial

public:
	// Constructors
	Polynomial() { p = NULL, degree = -1; };
	Polynomial(int, bool);
	Polynomial(int*, int);
	Polynomial(const Polynomial&); // Copy
	virtual ~Polynomial() { delete[] p; };
	// Methods
	void Mod(int); // Mod
	void Multy(int); // Multiplication x^p
	bool isIrreducible(int); // Check irreducible
	bool isPrimitive(int); // Check primitive
	bool isZero(); // Check polynomial is zero

	// Operator overloads
	Polynomial operator + (const Polynomial&) const;
	Polynomial operator - (const Polynomial&) const;
	Polynomial operator * (const Polynomial&) const;
	pair<Polynomial, Polynomial> operator / (const Polynomial&) const;
	Polynomial operator ^ (int); // Exponentiation
	Polynomial& operator = (const Polynomial &);
	bool operator == (const Polynomial &);

	friend ostream &operator<<(ostream &, Polynomial &);
};

int main()
{
	cout << "Write the degree of the polynomial: ";
	int deg1;
	cin >> deg1;
	NumberIrreducible(deg1, 2);
	
	cout << endl;
	return 0;
}

// Methods, Constructors

Polynomial::Polynomial(int deg, bool bin=false)
{
	if (!bin)
	{
		degree = deg;
		p = new int[deg + 1];
		for (int i = 0; i <= deg; i++)
			p[i] = 0;
	}
	else
	{
		degree = log2(deg);
		p = DecToBin(deg);
	}
}

Polynomial::Polynomial(int *coff, int deg)
{
	degree = deg;
	p = new int[deg + 1];
	for (int i = 0; i <= deg; i++)
		p[i] = coff[i];
	if (coff[deg] == 0) coff[deg] = 1;
}

Polynomial::Polynomial(const Polynomial& v)
{
	degree = v.degree;
	// allocating memory and creating a copy of the array
	p = new int[degree + 1];
	for (int i = 0; i <= degree; i++)
		p[i] = v.p[i];
}

Polynomial& Polynomial :: operator = (const Polynomial& v)
{
	if (degree != v.degree)
	{
		delete p;
		degree = v.degree;
		p = new int[degree + 1];
	}
	for (int i = 0; i <= degree; i++)
		p[i] = v.p[i];

	return *this;
}

Polynomial Polynomial::operator + (const Polynomial& a) const
{
	Polynomial result;
	if (degree > a.degree)
		result = Polynomial(*this);
	else
		result = Polynomial(a);
	for (int i = 0; i <= min(degree, a.degree); i++)
		result.p[i] = p[i] + a.p[i];
	return result;
}

Polynomial Polynomial::operator - (const Polynomial& a) const
{
	Polynomial result;
	if (degree >= a.degree)
		result = Polynomial(*this);
	else
		result = Polynomial(a);
	for (int i = 0; i <= min(degree, a.degree); i++)
		result.p[i] = p[i] - a.p[i];
	return result;
}

Polynomial Polynomial::operator * (const Polynomial& a) const
{
	Polynomial result = Polynomial(degree + a.degree);
	for (int i = 0; i <= degree; i++)
		for (int j = 0; j <= a.degree; j++)
			result.p[i + j] += p[i] * a.p[j];
	return result;
}

pair<Polynomial, Polynomial> Polynomial::operator / (const Polynomial& a) const
{
	pair<Polynomial, Polynomial> t = pair<Polynomial, Polynomial>();
	t.first = *this;
	t.second = *this;
	if (a.degree > degree) return t;
	Polynomial remainder = Polynomial(*this);
	Polynomial quotient = Polynomial(degree - a.degree, false);
	for (int i = 0; i < degree - a.degree + 1; i++)
	{
		double coeff = remainder.p[remainder.degree - i] / a.p[a.degree];
		quotient.p[quotient.degree - i] = coeff;
		for (int j = 0; j < a.degree + 1; j++)
		{
			remainder.p[remainder.degree - i - j] -= coeff * a.p[a.degree - j];
		}
	}
	
	quotient.Mod(2);
	remainder.Mod(2);

	t.first = quotient; // Quotient
	t.second = remainder; // Remains
	return t;
}

Polynomial Polynomial::operator ^ (int k)
{
	Polynomial temp = *this;
	for (int i = 1; i < k; i++)
		*this = *this * temp;
	return *this;
}

bool Polynomial::operator == (const Polynomial& a)
{
	if (degree != a.degree) return false;
	for (int i = 0; i <= degree; i++)
		if (p[i] != a.p[i]) return false;
	return true;
}

ostream &operator<<(ostream &fo, Polynomial &o)
{
	for (int i = 0; i < o.degree; i++)
		fo << o.p[i] << "X^" << i << "+";
	fo << o.p[o.degree] << "X^" << o.degree << endl;
	return fo;
}


void Polynomial::Mod(int fild)
{
	for (int i = 0; i <= degree; i++)
		p[i] %= fild;
}

void Polynomial::Multy(int k)
{
	int *temp = new int[degree + k + 1]{ 0 };
	for (int i = 0; i <= degree; i++)
		temp[degree + k - i] = p[degree - i];
	delete[] p;
	p = temp;
	degree += k;
}

bool Polynomial::isZero()
{
	bool flag = true;
	for (int i = 0; i <= degree; i++)
		if (p[i] != 0) flag = false;
	return flag;
}

bool Polynomial::isIrreducible(int fild)
{
	if (p[0] != 1) return false; // Free should be term (X^p + .. + 1)
	int k = 0;
	for (int i = 0; i <= degree; i++)
		if (p[i] != 0) k++;
	if (k % 2 == 0) return false; // The number of non-zero terms is odd
	if (degree == 1 && (p[0] + p[1]) == 2) return true; // 1 + X


	// Finding the decomposition
	int step = degree;
	for (int i = 1; i < step; i++) // (x^p + x^p-1 + ...)^k
		if (step % i == 0)
		{
			for (int s = pow(2, i), end = pow(2, i + 1); s < end; s++)
			{
				Polynomial t = Polynomial(s, true);
				Polynomial p = t ^ (step / i);
				p.Mod(fild);

				if (*this == p) return false;
			}
		}
	int i = 1, j = step - 1;
	while (i <= j) // (x^n + x^n-1 + ...) * (x^m + x^m-1 + ...) 
	{
		for (int step_i = pow(2, i), end = pow(2, i + 1); step_i < end; step_i++)
			for (int step_j = pow(2, j), end = pow(2, j + 1); step_j < end; step_j++)
			{
				Polynomial p1 = Polynomial(step_i, true);
				Polynomial p2 = Polynomial(step_j, true);
				Polynomial t = p1 * p2;
				t.Mod(2);

				if (*this == t) return false;
			}

		i++, j--;
	}

	return true;
}

// Functions

int* DecToBin(int num)
{
	int n = num;
	int k = log2(n) + 1;
	int* mass = new int[k];
	for (int i = 0; i < k; i++)
	{
		mass[i] = n % 2;
		n /= 2;
	}
	return mass;
}

void NumberIrreducible(int n, int fild)
{
	int kol = 0;
	for (int s = pow(2, n), end = pow(2, n + 1); s < end; s++)
	{
		Polynomial t = Polynomial(s, true);
		if (t.isIrreducible(fild))
		{
			cout << t;
			kol++;
		}
	}
	cout << "Number of irreducible polynomials of degree " << n << " - " << kol << endl;
}