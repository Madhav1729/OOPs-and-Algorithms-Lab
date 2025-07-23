#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <set>

class GraphAlgorithm
{
    public:
        GraphAlgorithm(std::vector<std::vector<int>>& adjList, std::vector<int>& hypeScores) 
        {
            adjList_ = adjList;
            hypeScores_ = hypeScores;
            nodes = adjList_.size();
        }

        GraphAlgorithm() {}

        virtual void Query() = 0;

    protected:
        int nodes;
        std::vector<std::vector<int>> adjList_;
        std::vector<int> hypeScores_; 
};

class isCycle : public GraphAlgorithm
{
    public:
        isCycle(std::vector<std::vector<int>>& adjList, std::vector<int>& hypeScores) : GraphAlgorithm(adjList, hypeScores) 
        {
            //check cycle
            checkCycle = false;
            std::vector<int> vis(adjList_.size());
            std::function<void(int)> dfs = [&](int u)
            {
                vis[u] = 1;
                for (auto &v : adjList_[u])
                {
                    if (vis[v] == 0) dfs(v);
                    else if (vis[v] == 1) 
                    {
                        checkCycle = true;
                    }
                }
                vis[u] = 2;
            };

            for (int i = 0; i < nodes; i++)
            {
                if (vis[i] == 0) dfs(i);
                if (checkCycle) break; 
            }
        }
        isCycle() : GraphAlgorithm() {}

        void Query()
        {
            std::cout << (checkCycle ? "YES\n" : "NO\n");
        }
    
    protected:
        bool checkCycle;
        
};

class validOrder : public isCycle
{
    public:
        validOrder(std::vector<std::vector<int>>& adjList, std::vector<int>& hypeScores) : isCycle(adjList, hypeScores)
        {
            for (int i = 0; i < nodes; i++)
            {
                std::sort(adjList_[i].begin(), adjList_[i].end(), std::greater<int>());
            }
            std::vector<bool> visited(nodes);
            std::function<void(int)> dfs = [&](int u)
            {
                visited[u] = true;
                for (auto &v : adjList_[u])
                {
                    if (!visited[v]) dfs(v);
                }
                ordering.push_back(u);
            };

            for (int i = nodes - 1; i >= 0; i--)
            {
                if (!visited[i]) dfs(i);
            }

            std::reverse(ordering.begin(), ordering.end());
        }
        validOrder() : isCycle() {}

        void Query()
        {
            if (checkCycle) 
            {
                std::cout << "NO\n";
                return;
            }

            for (auto& x : ordering)
            {
                std::cout << x + 1 << " ";
            }
            std::cout << std::endl;
        }

    protected:
        std::vector<int> ordering;
};

class indepComponent : public validOrder
{
    public:
        indepComponent(std::vector<std::vector<int>>& adjList, std::vector<int>& hypeScores) : validOrder(adjList, hypeScores)
        {
            //implement SCC
            if (!checkCycle)
            {
                newAdjList.resize(nodes);
                for (int i = 0; i < nodes; i++)
                {
                    for (auto &v : adjList_[i])
                    {
                        newAdjList[i].insert(v);
                    }
                }
                hyperScores.resize(nodes);
                for (int i = 0; i < nodes; i++) hyperScores[i] = hypeScores_[i];
                noOfSCCs = nodes;
                maxCardinality = 1;
                return;
            }

            std::vector<std::vector<int>> reverseAdjList(nodes);
            for (int i = 0; i < nodes; i++)
            {
                for(auto &v : adjList_[i])
                {
                    reverseAdjList[v].push_back(i);
                }
            }

            noOfSCCs = 0;
            int size = 0;
            maxCardinality = 0;
            std::vector<int> SCCGroup(nodes);
            std::vector<bool> visited(nodes);
            std::function<void(int)> dfs = [&](int u)
            {
                visited[u] = true;
                SCCGroup[u] = noOfSCCs;
                size++;
                for (auto &v : reverseAdjList[u])
                {
                    if (!visited[v]) dfs(v);
                }
            };

            for (auto &i : ordering)
            {
                if (!visited[i])
                {
                    dfs(i);
                    maxCardinality = std::max(maxCardinality, size);
                    size = 0;
                    noOfSCCs++;
                }
            }

            newAdjList.resize(noOfSCCs);
            hyperScores.resize(noOfSCCs);
            for (int i = 0; i < nodes; i++)
            {
                for (auto &v : adjList_[i])
                {
                    if (SCCGroup[i] != SCCGroup[v]) newAdjList[SCCGroup[i]].insert(SCCGroup[v]);
                }
                hyperScores[SCCGroup[i]] += hypeScores_[i];
            }
            
        }
        indepComponent() : validOrder() {}

        void Query()
        {
            std::cout << noOfSCCs << " " << maxCardinality << std::endl;
        }

        protected:
            int noOfSCCs;
            int maxCardinality;
            std::vector<std::set<int>> newAdjList;
            std::vector<long long> hyperScores;
};

class maxHype : public indepComponent
{
    public:
        maxHype(std::vector<std::vector<int>>& adjList, std::vector<int>& hypeScores) : indepComponent(adjList, hypeScores)
        {
            //find max hype
            std::vector<bool> checkIndeg(noOfSCCs, true);
            for (int i = 0; i < noOfSCCs; i++)
            {
                for (auto &v : newAdjList[i])
                {
                    checkIndeg[v] = false;
                }
            }

            std::vector<long long> dp(noOfSCCs, -1);
            std::function<long long(int)> dfs = [&](int u)
            {
                if (dp[u] != -1) return dp[u];
                dp[u] = hyperScores[u];
                long long mx = 0;
                for (auto &v : newAdjList[u])
                {
                    mx = std::max(mx, dfs(v));
                }
                dp[u] += mx;
                return dp[u];
            };

            mHype = 0;
            for (int i = 0; i < noOfSCCs; i++)
            {
                if (checkIndeg[i])
                {
                    mHype = std::max(mHype, dfs(i));
                }
            }
        }
        maxHype() : indepComponent() {}

        void Query()
        {
            std::cout << mHype << std::endl;
        }

    private:
        long long mHype;
};

int main()
{
    int n, m;
    std::cin >> n >> m;
    std::vector<int> hypeScores(n);
    std::vector<std::vector<int>> adjList(n);

    for (auto &x : hypeScores) std::cin >> x;
    for (int i = 0; i < m; i++)
    {
        int u, v;
        std::cin >> u >> v;
        u--; v--;
        adjList[u].push_back(v);
    }

    maxHype myGraphForMaxHype(adjList, hypeScores);

    int q;
    std::cin >> q;
    while(q--)
    {
        int type;
        std::cin >> type;
        if (type == 1)
        {
            myGraphForMaxHype.isCycle::Query();
        }
        else if (type == 2)
        {
            myGraphForMaxHype.indepComponent::Query();
        }
        else if (type == 3)
        {
            myGraphForMaxHype.validOrder::Query();
        }
        else 
        {
            myGraphForMaxHype.Query();
        }
    }
}