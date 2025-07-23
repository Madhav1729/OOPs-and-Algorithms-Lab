#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>

class Complex
{
    public:
        Complex(long long r, long long i) : r_(r), i_(i) {}
        Complex() 
        {
            r_ = 0;
            i_ = 0;
        }

        Complex& operator*(Complex& b)
        {
            long long temp;
            temp = r_ * b.r_ - i_ * b.i_;
            i_ = r_ * b.i_ + i_ * b.r_;
            r_ = temp;
            return *this;
        }

        Complex operator+(Complex& b)
        {
            return Complex(r_ + b.r_, i_ + b.i_);
        }

        Complex operator-(Complex& b)
        {
            return Complex(r_ - b.r_, i_ - b.i_);
        }

        friend std::istream& operator>>(std::istream& in, Complex& a);
        friend std::ostream& operator<<(std::ostream& out, Complex& a);

    private:
        long long r_;
        long long i_;
};

std::istream& operator>>(std::istream& in, Complex& a)
{
    in >> a.r_ >> a.i_;
    return in;
}

std::ostream& operator<<(std::ostream& out, Complex& a)
{
    out << a.r_ << " " << a.i_;
    return out;
}

template <typename T>
class PolynomialMultiplication;

template <typename T>
class Polynomial
{
    public:
        Polynomial(int n) : poly(n) {}
        Polynomial(std::vector<T>& p) : poly(p) {}
        Polynomial() {}

        Polynomial<T> operator*(Polynomial<T>& poly2)
        {
            PolynomialMultiplication<T> multiPoly;
            return multiPoly.multiply(*this, poly2);
        }

        inline int getLen()
        {
            return poly.size();
        }

        void printPoly()
        {
            for (auto& x : poly)
            {
                std::cout << x << " ";
            }
            std::cout << std::endl;
        }

        template <typename P> 
        friend std::istream& operator>>(std::istream& in, Polynomial<P>& a);

        template <typename P>
        friend class PolynomialMultiplication;

        template <typename P>
        friend class PolynomialEvaluation;

        template <typename P> 
        friend class PolynomialDifferentiation;

    private:
        std::vector<T> poly;
};

template<>
void Polynomial<long double>::printPoly()
{
    for (auto& s : poly)
    {
        std::cout << std::fixed << std::setprecision(6) << s << " ";
    }
    std::cout << std::endl;
}

template <typename T>
std::istream& operator>>(std::istream& in, Polynomial<T>& a)
{
    int n = a.poly.size();
    for (int i = 0; i < n; i++)
    {
        in >> a.poly[i];
    }

    return in;
}

template <typename T>
class PolynomialMultiplication
{
    public:
        PolynomialMultiplication() {}

        Polynomial<T> multiply(Polynomial<T>& poly1, Polynomial<T>& poly2)
        {
            //multiply p1 & p2 using D&C and return them
            std::vector<T> p1(poly1.poly), p2(poly2.poly);
            p1.resize(16);
            p2.resize(16);

            std::vector<T> res = multiply(p1, p2);
            res.resize(poly1.poly.size() + poly2.poly.size() - 1);

            return Polynomial<T>(res);
        }

        std::vector<T> multiply(std::vector<T> p1, std::vector<T> p2)
        {
            if (p1.size() == 1 || p2.size() == 1)
            {
                std::vector<T> res(p1.size() + p2.size() - 1);
                for (int i = 0; i < (int)p1.size(); i++)
                {
                    for (int j = 0; j < (int)p2.size(); j++)
                    {
                        res[i + j] = res[i + j] +  p1[i] * p2[j];
                    }
                }
                return res;
            }

            int mid = p1.size()/2;

            std::vector<T> res11 = multiply(std::vector<T>(p1.begin(), p1.begin() + mid), std::vector<T>(p2.begin(), p2.begin() + mid));
            std::vector<T> res22 = multiply(std::vector<T>(p1.begin() + mid, p1.end()), std::vector<T>(p2.begin() + mid, p2.end()));
            
            std::vector<T> p1half(mid);
            std::vector<T> p2half(mid);
            for (int i = 0; i < mid; i++)
            {
                p1half[i] = p1[i] + p1[mid + i];
            }
            for (int i = 0; i < mid; i++)
            {
                p2half[i] = p2[i] + p2[mid + i];
            }

            std::vector<T> res12 = multiply(p1half, p2half);

            std::vector<T> res(p1.size() + p2.size() - 1);
            for (int i = 0; i < 2 * mid - 1; i++)
            {
                res[i] = res[i] + res11[i];
                res[i + mid] = res[i + mid] + res12[i] - res11[i] - res22[i];
                res[i + mid + mid] = res[i + mid + mid] + res22[i];
            }
            return res;
        }
};

template <typename T>
class PolynomialEvaluation
{
    public:
        PolynomialEvaluation() {}

        T evaluate(Polynomial<T>& poly, long long x)
        {
            //evaluate the poly at x 
            T res = 0;

            T val = (T)1;
            for (auto& e : poly.poly)
            {
                res += e * val;
                val *= x;
            }
            return res;
        }
};

template <>
std::string PolynomialEvaluation<std::string>::evaluate(Polynomial<std::string>& poly, long long x)
{
    std::string res;

    for (int i = poly.getLen() - 1; i >=0; i--)
    {
        for (int j = 0; j < (long long)pow(x,i); j++)
        {
            res += poly.poly[i];
        }
    }
    return res;
}

template <typename T>
class PolynomialDifferentiation
{
    public:
        PolynomialDifferentiation() {}

        void differentiatePoly(Polynomial<T>& poly)
        {
            //diff the poly
            for (int i = 1; i < poly.getLen(); i++)
            {
                poly.poly[i] *= i;
            }
            poly.poly.erase(poly.poly.begin());
        };
};

int main()
{
    int q;
    std::cin >> q;
    int type;
    std::string dataType;

    while(q--)
    {
        std::cin >> type;
        if (type == 1)
        {
            std::cin >> dataType;

            if (dataType == "integer")
            {
                int deg;
                std::cin >> deg;

                Polynomial<long long> p1(deg);
                std::cin >> p1;

                std::cin >> deg;
                Polynomial<long long> p2(deg);
                std::cin >> p2;

                Polynomial<long long> product = p1 * p2;
                product.printPoly();
            }
            else if (dataType == "float")
            {
                int deg;
                std::cin >> deg;
                
                Polynomial<long double> p1(deg);
                std::cin >> p1;

                std::cin >> deg;
                Polynomial<long double> p2(deg);
                std::cin >> p2;

                Polynomial<long double> product = p1 * p2;
                product.printPoly();
            }
            else
            {
                int deg;
                std::cin >> deg;

                Polynomial<Complex> p1(deg);
                std::cin >> p1;

                std::cin >> deg;
                Polynomial<Complex> p2(deg);
                std::cin >> p2;

                Polynomial<Complex> product = p1 * p2;
                product.printPoly();
            }
        }
        else if (type == 2)
        {
            std::cin >> dataType;
            
            int deg;
            std::cin >> deg;
    
            if (dataType == "integer")
            {
                Polynomial<long long> poly(deg);
                std::cin >> poly;

                int x;
                std::cin >> x;

                PolynomialEvaluation<long long> evalPoly;
                std::cout << evalPoly.evaluate(poly, x) << std::endl;
            }
            else if (dataType == "float")
            {
                Polynomial<long double> poly(deg);
                std::cin >> poly;

                int x;
                std::cin >> x;

                PolynomialEvaluation<long double> evalPoly;
                std::cout << std::fixed << std::setprecision(6) << evalPoly.evaluate(poly, x) << std::endl;
            }
            else
            {
                Polynomial<std::string> poly(deg);
                std::cin >> poly;

                int x;
                std::cin >> x;

                PolynomialEvaluation<std::string> evalPoly;
                std::cout << evalPoly.evaluate(poly, x) << std::endl;
            }
        }
        else
        {
            std::cin >> dataType;

            int deg;
            std::cin >> deg;
    
            if (dataType == "integer")
            {
                Polynomial<long long> poly(deg);
                std::cin >> poly;

                PolynomialDifferentiation<long long> diffPoly;
                diffPoly.differentiatePoly(poly);
                poly.printPoly();
            }
            else
            {
                Polynomial<long double> poly(deg);
                std::cin >> poly;

                PolynomialDifferentiation<long double> diffPoly;
                diffPoly.differentiatePoly(poly);
                poly.printPoly();
            }
        }
    }
}

