#include <bits/stdc++.h>
using namespace std;
#define int long long //to avoid overflow issues

class Employee
{
    public:
        Employee(int employeeId, int age, int experience, int bossId)
        {
            employeeID_ = employeeId;
            age_ = age;
            experience_ = experience;
            bossID_ = bossId;
        }

        Employee() {}

        int getID() 
        {
            return employeeID_;
        }

        int getAge()
        {
            return age_;
        }

        int getExperience()
        {
            return experience_;
        }

        int getBossId()
        {
            return bossID_;
        }

    private:
        int employeeID_;
        int age_;
        int experience_;
        int bossID_;
};

//comparator for type 1 query
class Comparator
{
    public:
        Comparator(int t1, int t2) : type1(t1), type2(t2) {}
        Comparator() {}

        bool operator()(array<int,4>& a, array<int,4>& b)
        {
            if (a[type1] == b[type1]) return a[type2] < b[type2];
            return a[type1] < b[type1];
        }

    private:
        int type1;
        int type2;
};

//sortedEmployees is sorted wrt second criteria in a type 1 query and then sorted wrt to first criteria in case of tie breakers
int countNonDominatable(vector<array<int,4>>& sortedEmployees, int type)
{
    int mx = 0;
    int count = 0;
    int n = sortedEmployees.size();
    for (int i = 0; i < n; i++)
    {
        if (mx <= sortedEmployees[i][type]) count++; //counts if first half of criteria in a type 1 query is violated
        mx = max(mx, sortedEmployees[i][type]);
    }
    
    return count;
}

int32_t main()
{
    int n, q;
    cin >> n;
    vector<Employee> employees(n);
    unordered_map<int, int> IdMapping; //maps id to index of employee in the vector
    int founderId = -1;
    for (int i = 0; i < n; i++)
    {
        int employeeId, age, experience, bossId;
        cin >> employeeId >> age >> experience >> bossId;
        Employee newEmployee(employeeId, age, experience, bossId);
        employees[i] = newEmployee;
        IdMapping[employeeId] = i;
        if (employeeId == bossId) founderId = employeeId;
    }

    //precalc q to check if type 3 exists
    cin >> q;
    vector<vector<int>> queries(q);
    bool is3Present = false;
    for(int i = 0; i <q; i++)
    {
        int typ;
        cin >> typ;
        queries[i].push_back(typ);
        if (typ == 1)
        {
            int criteria;
            cin >> criteria;
            queries[i].push_back(criteria);
        }
        else if (typ == 2)
        {
            int id;
            cin >> id;
            queries[i].push_back(id);
        }
        else 
        {
            int id, k;
            cin >> id >> k;
            queries[i].push_back(id);
            queries[i].push_back(k);
            is3Present = true;
        }       
    }

    vector<int> employeeIds(n);
    for (int i = 0; i < n; i++)
    {
        employeeIds[i] = employees[i].getID();
    }
    sort(employeeIds.begin(), employeeIds.end());
    //compressing values of employeeids in the range of [0,n)
    unordered_map<int,int> newIDs;
    for (int i = 0; i < n; i++)
    {
        newIDs[employeeIds[i]] = i;
    }

    vector<vector<int>> adjList(n);
    vector<int> depth(n);
    vector<long long> subordinateSum(n);
    vector<int> subordinateSize(n);
    vector<vector<int>> subordinateList(n); //list of employees in its subtree

    for (int i = 0; i < n; i++)
    {
        if(employees[i].getBossId() != employees[i].getID()) adjList[newIDs[employees[i].getBossId()]].push_back(newIDs[employees[i].getID()]);
    }

    int dep = 0;
    function<void(int)> dfs = [&](int u) //dfs on the employee tree
    {
        depth[u] = dep;
        dep++;
        for (auto &v : adjList[u])
        {
            if (u != v) 
            {
                dfs(v);

                if(is3Present) 
                for (auto &x : subordinateList[v])
                {
                    subordinateList[u].push_back(x);
                }
                subordinateSum[u] += subordinateSum[v];
                subordinateSize[u] += subordinateSize[v];
            }
        }
        dep--;
        //computing only if type 3 query exists
        if(is3Present) subordinateList[u].push_back(u);
        subordinateSum[u] += employees[IdMapping[employeeIds[u]]].getExperience();
        subordinateSize[u]++;
    };
    dfs(newIDs[founderId]);

    //computing only if type 3 query exists
    if (is3Present)
    {
        for (int i = 0; i < n; i++)
        {
            sort(subordinateList[i].begin(), subordinateList[i].end(),[&](int a, int b){
                if(employees[IdMapping[employeeIds[a]]].getAge() != employees[IdMapping[employeeIds[b]]].getAge()) return employees[IdMapping[employeeIds[a]]].getAge() < employees[IdMapping[employeeIds[b]]].getAge();
                else return employeeIds[a] < employeeIds[b];
            }); //sorting subordinatie array of each employee wrt age
        }
    }

    //new vector for type 1 query sorting it later depending upon the type of type 1 query
    vector<array<int,4>> sortedEmployees(n);
    for (int i = 0; i < n; i++)
    {
        sortedEmployees[i] = {employees[i].getID(), employees[i].getAge(), employees[i].getExperience(), depth[newIDs[employees[i].getID()]]};
    }

    //answering queries
    cin >> q;
    for (int i = 0; i < q; i++)
    {
        int typ = queries[i][0];
        if (typ == 1)
        {
            int criteria = queries[i][1];
            int t1, t2;
            if (criteria == 1)
            {
                t1 = 3;
                t2 = 2;
            }
            else if (criteria == 2)
            {
                t1 = 3;
                t2 = 1;
            }
            else
            {
                t1 = 1;
                t2 = 2;
            }
            //sorts the array according to the type of type 1
            Comparator comp(t1, t2);
            sort(sortedEmployees.begin(), sortedEmployees.end(), comp);
            cout << countNonDominatable(sortedEmployees, t2) << endl;
        }
        else if (typ == 2)
        {
            int id = queries[i][1];
            int newId = newIDs[id];
            cout << subordinateSum[newId]/(subordinateSize[newId]) << endl;
        }
        else 
        {
            int id = queries[i][1], k = queries[i][2];
            int newid = newIDs[id];
            if (k > subordinateSize[newid]) cout << -1 << endl;
            else
            {
                cout << employeeIds[subordinateList[newid][k - 1]] << endl;
            }
        }
    }

}