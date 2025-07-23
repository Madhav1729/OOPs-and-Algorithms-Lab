#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <climits>
#include <cassert>
#define int long long

class Comparator
{
public:
    Comparator(bool t) : type(t) {}
    bool operator()(int a, int b)
    {
        if (type)
        {
            // ascending
            return a < b;
        }
        else
        {
            // descending
            return a > b;
        }
    }

private:
    bool type;
};

class Matrix
{
public:
    Matrix(int n) : n_(n), mat_(n, std::vector<int>(n)), count_(-1)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                std::cin >> mat_[i][j];
            }
        }
    }

    Matrix()
    {
    }
    // counts inversions
    long long countInversions(std::vector<int> &b, int l, int r, std::vector<int>& tempb)
    {
        if (l == r)
            return 0LL;

        int mid = (l + r) / 2;
        long long countLeft = countInversions(b, l, mid, tempb);
        long long countRight = countInversions(b, mid + 1, r, tempb);

        int i = l, j = mid + 1;
        int k = 0;
        long long count = 0;
        while (i <= mid && j <= r)
        {
            if (b[i] <= b[j])
            {
                tempb[k++] = b[i];
                i++;
            }
            else
            {
                count += mid - i + 1;
                tempb[k++] = b[j];
                j++;
            }
        }
        while (i <= mid)
            tempb[k++] = b[i++];
        while (j <= r)
            tempb[k++] = b[j++];
        assert(tempb.size() >= r - l + 1);
        for (int k = 0; k < r - l + 1; k++)
            b[k + l] = tempb[k];
        return count + countLeft + countRight;
    }

    void countInversions()
    {
        if (n_ <= 1) 
        {
            std::cout << 0 << std::endl;
            return;
        }
        if (count_ != -1)
        {
            std::cout << count_ << std::endl;
            return;
        }
        std::vector<int> flattendMat(n_ * n_);
        for (int i = 0; i < n_; i++)
        {
            for (int j = 0; j < n_; j++)
            {
                flattendMat[i * n_ + j] = mat_[i][j];
            }
        }
        std::vector<int> temp(n_ * n_);
        count_ = countInversions(flattendMat, 0, n_ * n_ - 1, temp);
        std::cout << count_ << std::endl;
    }

    void sortRows(Comparator &comparator)
    {
        // sorts rows
        std::vector<int> temp(n_);
        for (int i = 0; i < n_; i++)
        {
            sortArray(mat_[i], 0, n_ - 1, comparator, temp);
        }
    }

    void sortArray(std::vector<int> &b, int l, int r, Comparator &comparator, std::vector<int>& tempb)
    {
        if (l == r)
            return;

        int mid = (l + r) / 2;
        sortArray(b, l, mid, comparator, tempb);
        sortArray(b, mid + 1, r, comparator, tempb);

        int i = l, j = mid + 1;
        int k = 0;
        while (i <= mid && j <= r)
        {
            if (comparator(b[i], b[j]))
            {
                tempb[k++] = b[i];
                i++;
            }
            else
            {
                tempb[k++] = b[j];
                j++;
            }
        }
        while (i <= mid)
            tempb[k++] = b[i++];
        while (j <= r)
            tempb[k++] = b[j++];
        assert(tempb.size() >= r - l + 1);

        for (int k = 0; k < r - l + 1; k++)
            b[k + l] = tempb[k];
    }

    void display()
    {
        for (int i = 0; i < n_; i++)
        {
            for (int j = 0; j < n_; j++)
            {
                std::cout << mat_[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

private:
    int n_;
    std::vector<std::vector<int>> mat_;
    long long count_;
};

struct Point
{
    int x;
    int y;
    int time;
};

class comparatorPoint
{
public:
    comparatorPoint(bool t) : type(t) {}
    bool operator()(const Point &a, const Point &b)
    {
        if (type)
        {
            return a.x < b.x;
        }
        else
        {
            return a.y < b.y;
        }
    }

private:
    bool type;
};

inline long long euclideanDistance(const Point &a, const Point &b)
{
    return 1LL * (a.x - b.x) * (a.x - b.x) + 1LL * (a.y - b.y) * (a.y - b.y);
}

void MergeSort(std::vector<Point> &b, int l, int r, comparatorPoint &comparator, std::vector<Point> &tempb)
{
    if (l == r)
        return;

    int mid = (l + r) / 2;
    MergeSort(b, l, mid, comparator, tempb);
    MergeSort(b, mid + 1, r, comparator, tempb);

    int i = l, j = mid + 1;
    int k = 0;
    while (i <= mid && j <= r)
    {
        if (comparator(b[i], b[j]))
        {
            tempb[k++] = b[i];
            i++;
        }
        else
        {
            tempb[k++] = b[j];
            j++;
        }
    }
    while (i <= mid)
        tempb[k++] = b[i++];
    while (j <= r)
        tempb[k++] = b[j++];

    assert(tempb.size() >= r - l + 1);

    for (int k = 0; k < r - l + 1; k++)
        b[k + l] = tempb[k];
}

class Players
{
public:
    std::pair<Point, Point> closestPair(int l, int r)
    {
        if (r - l + 1 <= 3)
        {
            assert(l != r);
            std::pair<Point, Point> ans;
            long long mn = LLONG_MAX;
            for (int i = l; i < r; i++)
            {
                long long dis = euclideanDistance(pairsX[i], pairsX[i + 1]);
                if (mn > dis)
                {
                    mn = dis;
                    if (pairsX[i].time < pairsX[i + 1].time)
                        ans = {pairsX[i], pairsX[i + 1]};
                    else
                        ans = {pairsX[i + 1], pairsX[i]};
                }
                else if (mn == dis)
                {
                    Point fp = (pairsX[i].time < pairsX[i + 1].time ? pairsX[i] : pairsX[i + 1]);
                    Point sp = (pairsX[i].time > pairsX[i + 1].time ? pairsX[i] : pairsX[i + 1]);
                    if (ans.first.x != fp.x)
                    {
                        if (fp.x < ans.first.x)
                            ans = {fp, sp};
                    }
                    else if (ans.first.y != fp.y)
                    {
                        if (fp.y < ans.first.y)
                            ans = {fp, sp};
                    }
                    else
                    {
                        if (sp.x < ans.second.x)
                            ans = {fp, sp};
                    }
                }
            }
            if (mn > euclideanDistance(pairsX[r], pairsX[l]))
            {
                ans = {pairsX[l], pairsX[r]};
                if (pairsX[r].time < pairsX[l].time)
                    ans = {pairsX[r], pairsX[l]};
            }
            else if (mn == euclideanDistance(pairsX[r], pairsX[l]))
            {
                Point fp = (pairsX[r].time < pairsX[l].time ? pairsX[r] : pairsX[l]);
                Point sp = (pairsX[r].time > pairsX[l].time ? pairsX[r] : pairsX[l]);
                if (ans.first.x != fp.x)
                {
                    if (fp.x < ans.first.x)
                        ans = {fp, sp};
                }
                else if (ans.first.y != fp.y)
                {
                    if (fp.y < ans.first.y)
                        ans = {fp, sp};
                }
                else
                {
                    if (sp.x < ans.second.x)
                        ans = {fp, sp};
                }
            }
            return ans;
        }

        int mid = (l + r) / 2;
        std::pair<Point, Point> leftPair = closestPair(l, mid);
        std::pair<Point, Point> rightPair = closestPair(mid + 1, r);

        long long dis = std::min(euclideanDistance(leftPair.first, leftPair.second), euclideanDistance(rightPair.first, rightPair.second));
        std::vector<Point> midStrip;
        for (int i = l; i <= r; i++)
        {
            auto &[a,b,c] = pairsX[i];
            if (std::pow(std::abs(a - pairsX[mid].x), 2) <= dis)
                midStrip.push_back({a, b, c});
        }

        comparatorPoint comparator(false);
        std::vector<Point> temp(midStrip.size());
        MergeSort(midStrip, 0, midStrip.size() - 1, comparator, temp);

        std::pair<Point, Point> ans;
        long long mn = LLONG_MAX;
        for (int i = 0; i < (int)midStrip.size(); i++)
        {
            for (int j = i + 1; j < i + 8 && j < (int)midStrip.size(); j++)
            {
                long long tdis = euclideanDistance(midStrip[i], midStrip[j]);
                if (mn > tdis)
                {
                    mn = tdis;
                    if (midStrip[i].time < midStrip[j].time)
                        ans = {midStrip[i], midStrip[j]};
                    else
                        ans = {midStrip[j], midStrip[i]};
                }
                else if (mn == tdis)
                {
                    Point fp = (midStrip[i].time < midStrip[j].time ? midStrip[i] : midStrip[j]);
                    Point sp = (midStrip[i].time > midStrip[j].time ? midStrip[i] : midStrip[j]);
                    if (ans.first.x != fp.x)
                    {
                        if (fp.x < ans.first.x)
                            ans = {fp, sp};
                    }
                    else if (ans.first.y != fp.y)
                    {
                        if (fp.y < ans.first.y)
                            ans = {fp, sp};
                    }
                    else
                    {
                        if (sp.x < ans.second.x)
                            ans = {fp, sp};
                    }
                }
            }
        }

        if (dis < mn)
        {
            if (dis == euclideanDistance(leftPair.first, leftPair.second))
            {
                return leftPair;
            }
            else
                return rightPair;
        }
        else if (mn < dis)
        {
            return ans;
        }
        else
        {
            std::pair<Point, Point> sidePoints;
            if (dis == euclideanDistance(leftPair.first, leftPair.second))
            {
                sidePoints = leftPair;
            }
            else
                sidePoints = rightPair;

            if (sidePoints.first.x != ans.first.x)
            {
                if (sidePoints.first.x < ans.first.x)
                    return sidePoints;
            }
            else if (sidePoints.first.y != ans.first.y)
            {
                if (sidePoints.first.y < ans.first.y)
                    return sidePoints;
            }
            else if (sidePoints.second.x < ans.second.x)
                return sidePoints;
            return ans;
        }
    }

    void closestPair()
    {
        int n;
        std::cin >> n;
        std::vector<Point> pairs(n), pairs_;
        int i = 0;
        for (auto &[a, b, c] : pairs)
        {
            std::cin >> a >> b;
            c = i++;
        }

        std::vector<Point> temp(n);
        comparatorPoint comparator1(true);
        MergeSort(pairs, 0, n - 1, comparator1, temp);
        pairsX = pairs;

        comparatorPoint comparator2(false);
        MergeSort(pairs, 0, n - 1, comparator2, temp);
        pairsY = pairs;

        std::pair<Point, Point> ans = closestPair(0, n - 1);
        std::cout << ans.first.x << " " << ans.first.y << " " << ans.second.x << " " << ans.second.y << std::endl;
    }

private:
    std::vector<Point> pairsX, pairsY;
};

int32_t main()
{
    Matrix mat[3];
    Players play;
    int index;

    std::string type;

    while (true)
    {
        std::cin >> type;

        if (type == "CREATE_2D")
        {
            int n;
            std::cin >> n;
            mat[0] = Matrix(n);
            mat[1] = mat[0];
            mat[2] = mat[0];
            Comparator comparator1(true);
            Comparator comparator2(false);
            mat[1].sortRows(comparator1);
            mat[2].sortRows(comparator2);
            index = 0;
        }
        else if (type == "DISPLAY_2D")
        {
            mat[index].display();
        }
        else if (type == "SORT_2D")
        {
            std::string ad;
            std::cin >> ad;

            if (ad == "ascending")
            {
                index = 1;
            }
            else
            {
                index = 2;
            }
        }
        else if (type == "CLOSEST_2D")
        {
            play.closestPair();
        }
        else if (type == "INVERSION_2D")
        {
            mat[index].countInversions();
        }
        else
            break;
    }
}
