#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>

const int N = 1e6 + 1;
const int mod = 1000000007;

class Complex
{
    public:
        Complex() {}
        
        std::pair<double,double> add(std::pair<double,double> a, std::pair<double,double> b)
        {
            return {a.first + b.first, a.second + b.second};
        }

        std::pair<double,double> sub(std::pair<double,double> a, std::pair<double,double> b)
        {
            return {a.first - b.first, a.second - b.second};
        }

        std::pair<double,double> mult(std::pair<double,double> a, std::pair<double,double> b)
        {
            return {a.first * b.first - a.second * b.second, a.first * b.second + a.second * b.first};
        }

    private:
};

class Rational : public Complex
{
    public:
        Rational() : Complex() {}

        double add(double a, double b)
        {
            auto res = Complex::add({a,0}, {b,0});
            return res.first;
        }

        double sub(double a, double b)
        {
            auto res = Complex::sub({a,0}, {b,0});
            return res.first;
        } 

        double mult(double a, double b)
        {
            auto res = Complex::mult({a,0}, {b,0});
            return res.first;
        }

        std::pair<int,int> reduce(int num, int denom)
        {
            int Gcd = std::__gcd(num, denom);
            return {num/Gcd, denom/Gcd};
        }

    private:
};

class Number
{
    public: 
        Number()
        {
            isPrime.resize(N, true);
            for (int i = 2; i < N; i++)
            {
                for (int j = 2; j * i < N; j++)
                {
                    isPrime[i*j] = false;
                }
            }
        }

        long long BinaryExponentiation(long long a, long long b)
        {
            long long res = 1;
            while (b > 0)
            {
                if (b & 1) res *= a;
                a = a*a;
                a %= mod;
                res %= mod;
                b /= 2;
            }
            return res;
        }

        long long modInverse(long long a)
        {
            return BinaryExponentiation(a, mod - 2);
        }

        bool isPrimef(int n)
        {
            if (n == 1) return false;
            return isPrime[n];
        }

        int inverse(int n)
        {
            return modInverse(n);
        }

        private:
        std::vector<bool> isPrime;
};

int main()
{
    int op;
    std::cin >> op;

    Number num;
    Complex cNum;
    Rational rNum;

    for(int i = 0; i < op; i++)
    {
        std::string type;
        std::cin >> type;
        if (type == "complex")
        {
            std::string fun;
            std::cin >> fun;
            double a1, a2, b1, b2;
            std::cin >> a1 >> a2 >> b1 >> b2;
            if (fun == "add")
            {
                auto res = cNum.add({a1,a2}, {b1,b2});
                std::cout << std::fixed << std::setprecision(3) << res.first << " " << res.second << std::endl;
            }
            else if (fun == "sub")
            {
                auto res = cNum.sub({a1,a2}, {b1,b2});
                std::cout << std::fixed << std::setprecision(3) << res.first << " " << res.second << std::endl;
            }
            else 
            {
                auto res = cNum.mult({a1,a2}, {b1,b2});
                std::cout << std::fixed << std::setprecision(3) << res.first << " " << res.second << std::endl;
            }
        }
        else if (type == "rational")
        {
            std::string fun;
            std::cin >> fun;
            
            if (fun == "add")
            {
                double a1, a2, b1, b2;
                std::cin >> a1 >> a2 >> b1 >> b2;
                auto res = rNum.add(a1/a2, b1/b2);
                std::cout << std::fixed << std::setprecision(3) << res << std::endl;
            }
            else if (fun == "sub")
            {
                double a1, a2, b1, b2;
                std::cin >> a1 >> a2 >> b1 >> b2;
                auto res = rNum.sub(a1/a2, b1/b2);
                std::cout << std::fixed << std::setprecision(3) << res << std::endl;
            }
            else if (fun == "mult")
            {
                double a1, a2, b1, b2;
                std::cin >> a1 >> a2 >> b1 >> b2;
                auto res = rNum.mult(a1/a2, b1/b2);
                std::cout << std::fixed << std::setprecision(3) << res << std::endl;
            }
            else
            {
                int a, b;
                std::cin >> a >> b;
                int na = std::abs(a);
                int nb = std::abs(b);
                auto res = rNum.reduce(na, nb);
                if (std::min(a,b) < 0) std::cout << -res.first << " " << res.second << std::endl;
                else std::cout << res.first << " " << res.second << std::endl;
            } 
        }
        else
        {
            std::string fun;
            std::cin >> fun;
            int n;
            std::cin >> n;
            if (fun == "isprime")
            {
                std::cout << num.isPrimef(n) << std::endl;
            }
            else
            {
                std::cout << num.inverse(n) << std::endl;
            }
        }
    }
}