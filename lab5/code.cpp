#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <functional>
#include <random>
#include <algorithm>
const int mod = 1e9 + 7;

long long sqRoot(long long num)
{
    long long i = 0, j = num;
    while (i < j)
    {
        long long mid = (i + j) / 2;
        if (mid * mid < num)
            i = mid + 1;
        else
            j = mid;
    }
    return i;
}

class CurrencySorter
{
public:
    void operator()(std::vector<int> &a)
    {
        std::function<void(int, int)> quickSort = [&](int l, int r) -> void
        {
            if (l >= r)
                return;
            int idx = r;
            int pivot = a[idx];
            int i = l, j = r;
            while (i < j)
            {
                if (a[i] < pivot)
                    i++;
                else if (a[j] >= pivot)
                    j--;
                else
                {
                    std::swap(a[i], a[j]);
                    i++;
                    j--;
                }
            }
            if (a[i] < pivot)
                i++;
            j++;
            while (j <= r)
            {
                if (a[j] == pivot)
                {
                    std::swap(a[i], a[j]);
                    break;
                }
                j++;
            }
            quickSort(l, i - 1);
            quickSort(i + 1, r);
        };
        quickSort(0, a.size() - 1);
        for (auto &x : a)
        {
            std::cout << x << " ";
        }
        std::cout << std::endl;
    }

    int getRandomNumber(int low, int high)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(low, high);
        return dist(gen);
    }
};

class Matrix
{
    public:
        Matrix(int a1, int a2, int a3, int a4)
        {
            a[0] = a1;
            a[1] = a2; 
            a[2] = a3;
            a[3] = a4;
        }

        Matrix& operator*(Matrix& b)
        {
            long long var1, var2, var3, var4;
            var1 = a[0] * b.a[0] + a[1] * b.a[2];
            var2 = a[0] * b.a[1] + a[1] * b.a[3];
            var3 = a[2] * b.a[0] + a[3] * b.a[2];
            var4 = a[2] * b.a[1] + a[3] * b.a[3];
            a[0] = var1%mod;
            a[1] = var2%mod;
            a[2] = var3%mod;
            a[3] = var4%mod;
            return *this;
        }

        long long a[4];
};

class FibonacciGenerator
{
public:
    long long operator()(long long i)
    {
        Matrix a{1,1,1,0};
        Matrix res{1,0,0,1};
        res.a[1] = 0;
        while (i > 0)
        {
            if (i & 1) res * a;
            a * a;
            i >>= 1;
        }
        return res.a[1];
    };
};

class PrimeCalculator
{
public:
    PrimeCalculator()
    {
        std::vector<bool> vec(1e6 + 1, true);
        for (int i = 2; i <= 1e6; i++)
        {
            for (int j = 2; i * j <= 1e6; j++)
            {
                vec[i * j] = false;
            }
        }
        vec[0] = false;
        vec[1] = false;
        for (int i = 0; i < 1e6 + 1; i++)
        {
            if (vec[i])
                primes_.push_back(i);
        }
    }

    void printPrimes(long long l, long long r)
    {
        // prints primes
        std::vector<bool> primes(r - l + 1, true);
        assert(r - l + 1 <= 1e6);
        for (long long &i : primes_)
        {
            for (long long j = (l + i - 1) / i; i * j <= r; j++)
            {
                if (j == 1)
                    continue;
                primes[i * j - l] = false;
            }
        }

        for (int i = 0; i < r - l + 1; i++)
        {
            if (i + l <= 1)
                continue;
            if (primes[i])
                std::cout << 0LL + i + l << " ";
        }
        std::cout << std::endl;
    }

    long long printPrimeSum(long long l, long long r)
    {
        // return sum of primes
        long long sum = 0;
        std::vector<bool> primes(r - l + 1, true);
        assert(r - l + 1 <= 1e6);
        for (long long &i : primes_)
        {
            for (long long j = (l + i - 1) / i; i * j <= r; j++)
            {
                if (j == 1)
                    continue;
                primes[i * j - l] = false;
            }
        }

        for (int i = 0; i < r - l + 1; i++)
        {
            if (i + l <= 1)
                continue;
            if (primes[i])
                sum += 0LL + i + l;
        }
        return sum;
    }

private:
    std::vector<long long> primes_;
};

class NumberAnalyzer
{
public:
    NumberAnalyzer()
    {
    }

    bool isSquareFree(int n)
    {
        if (n%4 == 0) return false;
        for (int i = 3; i <= 1e4; i += 2)
        {
            if (n % i == 0)
            {
                int m = n;
                m /= i;
                if (m % i == 0) return false;
            }
        }
        return true;
    }

    void countDivisors(int n)
    {
        int ct = 0;
        int rootn = sqRoot(n);
        for (int i = 1; i <= rootn; i++)
        {
            if (i * i > n) break;
            if (n % i == 0)
            {
                ct += 2;
                if (n == i * i)
                    ct--;
            }
        }
        std::cout << ct << std::endl;
    }

    void sumOfDivisors(int n)
    {
        long long sum = 0;
        int rootn = sqRoot(n);
        for (int i = 1; i <= rootn; i++)
        {
            if (i * i > n)
                break;
            if (n % i == 0)
            {
                sum += i;
                sum += n / i;
                if (n == i * i)
                    sum -= i;
            }
        }
        std::cout << sum << std::endl;
    }

};

int main()
{
    NumberAnalyzer myNum;
    PrimeCalculator myPrime;
    CurrencySorter mySort;
    FibonacciGenerator myFib;
    int type;
    std::cin >> type;

    if (type == 1)
    {
        int t;
        std::cin >> t;
        while (t--)
        {
            int n;
            std::cin >> n;
            std::vector<int> a(n);
            for (int i = 0; i < n; i++)
                std::cin >> a[i];
            mySort(a);
        }
    }
    else if (type == 2)
    {
        int t;
        std::cin >> t;
        long long n;
        while (t--)
        {
            std::cin >> n;
            std::cout << myFib(n) << std::endl;
        }
    }
    else if (type == 3)
    {
        int t;
        std::cin >> t;
        std::string s;
        long long l, r;

        while (t--)
        {
            std::cin >> s >> l >> r;
            if (s == "printPrimes")
            {
                myPrime.printPrimes(l, r);
            }
            else
            {
                std::cout << myPrime.printPrimeSum(l, r) << std::endl;
            }
        }
    }
    else
    {
        int t;
        std::cin >> t;
        std::string s;
        int num;

        while (t--)
        {
            std::cin >> s >> num;

            if (s == "isSquareFree")
            {
                std::cout << (myNum.isSquareFree(num) ? "yes\n" : "no\n");
            }
            else if (s == "countDivisors")
            {
                myNum.countDivisors(num);
            }
            else
            {
                myNum.sumOfDivisors(num);
            }
        }
    }
}
