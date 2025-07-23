#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <algorithm>
#include <functional>

class Graph
{
    public:
        Graph() {}

        void addEdge(int u, int v)
        {
            adj[u].insert(v);
            adj[v].insert(u);
        }

        void removeEdge(int u, int v)
        {
            if (adj[u].find(v) == adj[u].end()) return;
            adj[u].erase(v);
            adj[v].erase(u);
        }

        void isReachable(int u, int v)
        {
            std::vector<bool> vis(adj.size());
            std::function<void(int,int)> dfs = [&](int x, int p)
            {
                vis[x] = true;
                for (auto &y : adj[x])
                {
                    if (!vis[y])
                    {   
                        dfs(y, x);
                    }
                }   
            };

            dfs(u, -1);
            if (vis[v]) std::cout << "Yes\n";
            else std::cout << "No\n";
        }

        Graph& operator+(Graph& graph)
        {
            std::cin >> graph;
            if (graph.adj.size() > adj.size()) adj.resize(graph.adj.size());
            for (int u = 0; u < graph.adj.size(); u++)
            {   
                for (auto &v : graph.adj[u])
                {
                    addEdge(u, v);
                }
            }
            return *this;
        }

        Graph& operator-(Graph& graph)
        {
            std::cin >> graph;
            Graph newGraph;
            newGraph.adj.resize(std::max(adj.size(), graph.adj.size()));
            //std::vector<std::vector<int>> adjMatrix(newGraph.adj.size(), std::vector<int>(newGraph.adj.size()));
            for (int u = 0; u < graph.adj.size(); u++)
            {
                for (auto &v : graph.adj[u])
                {
                    if (adj[u].find(v) != adj[u].end())
                    {
                        newGraph.addEdge(u,v);
                    }
                }
            }
            adj = newGraph.adj;
            return *this;
        }

        Graph& operator!()
        {
            Graph newGraph;
            newGraph.adj.resize(adj.size());
            for (int i = 0; i < adj.size(); i++)
            {
                for (int j = 0; j < adj.size(); j++)
                {
                    if (i == j) continue;
                    if (adj[i].find(j) == adj[i].end()) newGraph.addEdge(i, j);
                }
            }
            adj = newGraph.adj;
            return *this;
        }

        friend std::istream& operator>>(std::istream& stream, Graph& graph);
        friend std::ostream& operator<<(std::ostream& stream, Graph& graph);

    private:
        std::vector<std::set<int>> adj;
};

std::istream& operator>>(std::istream& stream, Graph& graph)
{
    int n, m;
    stream >> n >> m;
    graph.adj.resize(n);
    for (int i = 0; i < m; i++)
    {
        int u, v;
        stream >> u >> v;
        graph.addEdge(u,v);
    }
    return stream;
}

std::ostream& operator<<(std::ostream& stream, Graph& graph)
{
    for (int i = 0; i < graph.adj.size(); i++)
    {
        stream << "Vertex " << i << ": ";
        for (auto &v : graph.adj[i])
        {
            stream << v << " ";
        }
        stream << std::endl;
    }
    return stream;
}

int main()
{
    std::string s;
    std::cin >> s;
    Graph mGraph;
    std::cin >> mGraph;
    while(true)
    {
        std::cin >> s;
        if (s == "union")
        {
            std::cin >> s;
            Graph graph;
            mGraph + graph;
        }
        else if (s == "intersection")
        {
            std::cin >> s;
            Graph graph;
            mGraph - graph;
        }
        else if (s == "complement")
        {
            !mGraph;
        }
        else if (s == "isReachable")
        {
            int u, v;
            std::cin >> u >> v;
            mGraph.isReachable(u, v);
        }
        else if (s == "add_edge")
        {
            int u, v;
            std::cin >> u >> v;
            mGraph.addEdge(u, v);
        }
        else if (s == "remove_edge")
        {
            int u, v;
            std::cin >> u >> v;
            mGraph.removeEdge(u, v);
        }
        else if (s == "printGraph")
        {
            std::cout << mGraph;
        }
        else break;
    }
}