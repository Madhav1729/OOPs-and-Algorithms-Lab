#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <functional>

struct Point
{
    int x, y;

    Point(int x_x, int y_y) : x(x_x), y(y_y) {}
    Point() {}

    Point operator+(Point& b)
    {
        return Point(x + b.x, y + b.y);
    }

    Point operator/(int val)
    {
        return Point(x / val, y / val);
    }

};

long double distanceBtwPoints(Point& a, Point& b)
{
    return sqrtl((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

std::istream& operator>>(std::istream& in, Point& a)
{
    in >> a.x >> a.y;
    return in;
}

class Island
{
    public:
        Island() {}

        virtual void takeInput() = 0;

        std::string getId()
        {
            return id_;
        }

        friend bool findPath(std::vector<Island*>& islands, std::vector<std::string>& path);

    protected:
        std::string id_;
        Point centre_;
        long double farthestDistance_;
};

std::istream& operator>>(std::istream& in, Island& a)
{
    a.takeInput();
    return in;
}

class Rectangle : public Island
{
    public:
        Rectangle() : Island() {}

        void takeInput()
        {
            std::cin >> id_ >> p1 >> p2 >> p3 >> p4;
            centre_ = (p1 + p2 + p3 + p4) / 4;
            farthestDistance_ = distanceBtwPoints(centre_, p1);
        }

    private:
        Point p1, p2, p3, p4;
};

class Triangle : public Island
{
    public:
        Triangle() : Island() {}

        void takeInput()
        {
            std::cin >> id_ >> p1 >> p2 >> p3;
            centre_ = (p1 + p2 + p3) / 3;
            farthestDistance_ = std::max(distanceBtwPoints(centre_, p1), std::max(distanceBtwPoints(centre_, p2), distanceBtwPoints(centre_, p3)));
        }

    private:
        Point p1, p2, p3;
};

class Circle : public Island
{
    public: 
        Circle() : Island() {}

        void takeInput()
        {
            std::cin >> id_ >> centre_ >> radius_;
            farthestDistance_ = radius_;
        }
    
    private:
        int radius_;
};

bool findPath(std::vector<Island*>& islands, std::vector<std::string>& path)
{
    int n = islands.size();

    std::vector<std::vector<int>> adjList(n);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i == j) continue;

            if (distanceBtwPoints((*islands[i]).centre_, (*islands[j]).centre_) <= (*islands[i]).farthestDistance_ + (*islands[j]).farthestDistance_ + (1e-9))
            {
                adjList[i].push_back(j);
            }
        }
    }

    std::vector<std::vector<int>> dp((1LL << n) + 1, std::vector<int>(n, -1));
    std::function<int(int, int)> f = [&](int mask, int u)
    {
        if (dp[mask][u] != -1) return dp[mask][u];

        dp[mask][u] = 0;
        for (auto &v : adjList[u])
        {
            if ((mask & (1LL << v)) == 0)
            {
                dp[mask][u] = std::max(dp[mask][u], f(mask | (1LL << v), v));
            }
        }

        dp[mask][u]++;
        return dp[mask][u];
    };

    bool check = false;

    int mx = 0;
    int index = -1;
    for (int i = 0; i < n; i++)
    {
        f((1LL << i), i);
        if (dp[(1LL << i)][i] > mx) 
        {
            mx = dp[(1LL << i)][i];
            index = i;
            if (mx == n) 
            {
                dp.assign((1LL << n) + 1, std::vector<int>(n, -1));
                check = true;
                break;
            }
        }
        dp.assign((1LL << n) + 1, std::vector<int>(n, -1));
    }

    f((1LL << index), index);

    std::function<void(int, int)> backTrack = [&](int mask, int u)
    {
        path.push_back((*islands[u]).getId());

        for (auto &v : adjList[u])
        {
            if (((mask & (1LL << v)) == 0) && (dp[mask][u] == dp[mask | (1LL << v)][v] + 1))
            {
                backTrack(mask + (1LL << v), v);
                break;
            }
        }
    };

    backTrack((1LL << index), index);
    return check;
}

int main()
{
    int n;
    std::cin >> n;
    std::vector<Island*> islands(n);

    for (int i = 0; i < n; i++)
    {
        std::string type;
        std::cin >> type;
        if (type == "RECTANGLE") 
        {
            islands[i] = new Rectangle();
            std::cin >> (*islands[i]);
        }
        else if (type == "TRIANGLE")
        {
            islands[i] = new Triangle();
            std::cin >> (*islands[i]);
        }
        else
        {
            islands[i] = new Circle();
            std::cin >> (*islands[i]);
        }
    }

    std::vector<std::string> longestPath;
    bool check = findPath(islands, longestPath);

    std::cout << (check ? "YES\n" : "NO\n");

    if(!check) std::cout << longestPath.size() << std::endl;
    for (auto &x: longestPath)
    {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    for(auto& x : islands)
    {
        delete x;
    }
}
