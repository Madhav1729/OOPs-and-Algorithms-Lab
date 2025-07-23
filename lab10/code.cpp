#include <iostream>
#include <vector>
#include <map>

template <typename T>
class PriorityQueue
{
public:
    PriorityQueue() : size_(0), arr_(1) {}

    const size_t size() const
    {
        return size_;
    }

    const T &top() const
    {
        if (!empty())
            return arr_[1];
        else
            std::exit(0);
    }

    const bool empty() const
    {
        if (size_ == 0)
            return true;
        return false;
    }

    void push(const T &ele)
    {
        arr_.push_back(ele);
        size_++;
        int index = size_;
        while (index > 1 && arr_[index / 2] > ele)
        {
            arr_[index] = arr_[index / 2];
            index >>= 1;
        }
        arr_[index] = ele;
    }

    void pop()
    {
        if (empty())
        {
            return;
        }

        T ele = arr_[size_];
        size_--;
        arr_.pop_back();

        int index = 1;
        while (index * 2 + 1 <= size_)
        {
            if (arr_[index * 2] < arr_[index * 2 + 1])
            {
                if (ele > arr_[index * 2])
                {
                    arr_[index] = arr_[index * 2];
                    index *= 2;
                }
                else
                    break;
            }
            else
            {
                if (ele > arr_[index * 2 + 1])
                {
                    arr_[index] = arr_[index * 2 + 1];
                    index *= 2;
                    index++;
                }
                else
                    break;
            }
        }
        if (index * 2 <= size_ && ele > arr_[index * 2])
        {
            arr_[index] = arr_[index * 2];
            index *= 2;
        }

        arr_[index] = ele;
    }

private:
    std::vector<T> arr_;
    size_t size_;
};

struct node
{
    int d;
    int v;

    node(int a, int b) : d(a), v(b) {}
    node() {}

    bool operator>(const node &b)
    {
        return d > b.d;
    }

    bool operator<(const node &b)
    {
        return d < b.d;
    }
};

int32_t main()
{
    int n, m;
    std::cin >> n >> m;

    std::vector<std::string> rooms(n);
    std::map<std::string, int> roomId; // mapping strings to int
    for (int i = 0; i < n; i++)
    {
        std::cin >> rooms[i];
        roomId[rooms[i]] = i;
    }

    std::vector<std::vector<node>> adjList(n);
    for (int i = 0; i < m; i++)
    {
        std::string a, b;
        int c;
        std::cin >> a >> b >> c;
        adjList[roomId[a]].push_back(node(roomId[b], c));
        adjList[roomId[b]].push_back(node(roomId[a], c));
    }

    std::string source, end;
    std::cin >> source >> end;

    std::vector<int> distance(2 * n, 2e9 + 1); // treating reaching with even or odd no of edges to a node as two different nodes
    distance[roomId[source]] = 0;

    PriorityQueue<node> pq;
    pq.push(node(0, roomId[source]));

    while (!pq.empty())
    {
        auto [w, u] = pq.top();
        pq.pop();
        int u_ = u;
        if (u >= n)
            u_ -= n;
        if (distance[u] < w)
            continue;

        for (auto &[v, d] : adjList[u_])
        {
            int v_ = v;
            if (u < n)
                v_ += n;
            if (distance[v_] < w + d)
                continue;
            distance[v_] = w + d;
            pq.push(node(w + d, v_));
        }
    }

    if (distance[roomId[end]] == 2e9 + 1)
        std::cout << -1 << std::endl;
    else
        std::cout << distance[roomId[end]] << std::endl;
}