#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

template <typename T>
class Kingdom;

template <typename T>
class Comparator;

int senapatis;
int dandanayakas;

class Sentinel
{
    public:
        Sentinel(int id, std::string name, std::string rank) : id_(id), name_(name), rank_(rank) {}
        Sentinel() {}

        virtual int countHigherRanked(Kingdom<Sentinel*>& kingdom) = 0;

        template <typename T>
        friend class Kingdom;

        template <typename T>
        friend class Comparator;

    protected:
        int id_;
        std::string name_;
        std::string rank_;
};

class Senapati : public Sentinel
{
    public:
        Senapati(int id, std::string name, std::string rank) : Sentinel(id, name, rank) {}
        Senapati() : Sentinel() {}

        int countHigherRanked(Kingdom<Sentinel*>& kingdom) 
        {
            return 0;
        }
};

class Dandanayaka : public Sentinel
{
    public:
        Dandanayaka(int id, std::string name, std::string rank) : Sentinel(id, name, rank) {}
        Dandanayaka() : Sentinel() {}

        int countHigherRanked(Kingdom<Sentinel*>& kingdom)
        {
            return senapatis;
        }
};

class Chaturangini : public Sentinel
{
    public:
        Chaturangini(int id, std::string name, std::string rank) : Sentinel(id, name, rank) {}
        Chaturangini() : Sentinel() {}

        int countHigherRanked(Kingdom<Sentinel*>& kingdom)
        {
            return senapatis + dandanayakas;
        }
};

template <typename NodeType>
class Comparator
{
    public:
        bool operator()(NodeType& a, NodeType& b)
        {
            if (a->rank_ == "SENAPATI") 
            {
                if (b->rank_ == "SENAPATI") return a->id_ < b->id_;
                return true;
            }
            if (b->rank_ == "SENAPATI") return false;
            if (a->rank_ == "DANDANAYAKA")
            {
                if (b->rank_ == "DANDANAYAKA") return a->id_ < b->id_;
                return true;
            } 
            if (b->rank_ == "DANDANAYAKA") return false;
            return a->id_ < b->id_;
        }
};

template <typename NodeType>
class Kingdom
{
    public:
        Kingdom(std::vector<NodeType>& sentinels, std::vector<std::vector<int>>& roadsList) : sentinels_(sentinels), roadsList_(roadsList)
        {
            minActiveSentinels = -1;
            sortedSentinels_ = sentinels_;
            std::sort(sortedSentinels_.begin(), sortedSentinels_.end(), Comparator<NodeType>());
            senapatis = 0;
            dandanayakas = 0;
            for (auto &a : sortedSentinels_)
            {
                if (a->rank_ == "SENAPATI") senapatis++;
                else if (a->rank_ == "DANDANAYAKA") dandanayakas++;
                else break;
            }
        }
        Kingdom() {}

        int calculateMinActiveSentinels();

        void printSortedSentinels()
        {
            for (auto &x : sortedSentinels_)
            {
                std::cout << x->id_ << " ";
            }
            std::cout << std::endl;
        }

        template <typename P>
        friend class Comparator;

    private:
        std::vector<NodeType> sentinels_;
        std::vector<NodeType> sortedSentinels_;
        std::vector<std::vector<int>> roadsList_;
        int minActiveSentinels;
};

template <typename Nodetype>
int Kingdom<Nodetype>::calculateMinActiveSentinels()
{
    if (minActiveSentinels != -1) return minActiveSentinels;

    const int inf = 1e6;
    std::vector<std::vector<int>> dp(roadsList_.size(), std::vector<int>(2, inf));

    std::function<int(int,int,int)> dfs = [&](int u, int p, int taken)
    {
        
        if (dp[u][taken] != inf) return dp[u][taken];

        int count1 = 1;
        int count2 = 0;

        for (auto &v : roadsList_[u])
        {
            if (v == p) continue;
            count1 += dfs(v, u, 1);
            count2 += dfs(v, u, 0);
        }

        if (taken) 
        {
            if (count1 < count2) 
            {
                dp[u][taken] = count1;
            }
            else dp[u][taken] = count2;
        }
        else
        {
            dp[u][taken] = count1;
        }

        return dp[u][taken];
    };

    return dfs(0, -1, 1);
}

int main()
{
    int n;
    std::cin >> n;
    std::vector<std::vector<int>> roads(n);
    for (int i = 0; i < n - 1; i++)
    {
        int a, b;
        std::cin >> a >> b;
        roads[a].push_back(b);
        roads[b].push_back(a);
    }

    std::vector<Sentinel*> sentinels(n);
    for (int i = 0; i < n; i++)
    {
        std::string name, rank;
        std::cin >> name >> rank;
        if (rank == "SENAPATI") sentinels[i] = new Senapati(i, name, rank);
        else if (rank == "DANDANAYAKA") sentinels[i] = new Dandanayaka(i, name, rank);
        else sentinels[i] = new Chaturangini(i, name, rank);
    }

    Kingdom<Sentinel*> kingdom(sentinels, roads);

    int q;
    std::cin >> q;
    while (q--)
    {
        int type;
        std::cin >> type;
        if (type == 1)
        {
            std::cout << kingdom.calculateMinActiveSentinels() << std::endl;
        }
        else if (type == 2)
        {
            kingdom.printSortedSentinels();
        }
        else
        {
            int id;
            std::cin >> id;
            std::cout << sentinels[id]->countHigherRanked(kingdom) << std::endl;
        }
    }
}