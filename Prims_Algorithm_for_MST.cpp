#include<bits/stdc++.h>

using namespace std;

//random graph generation(weighted, connected, undirected)
vector<vector<pair<int, int>>> createRandomGraph(int n, int edges)
{
    vector<vector<pair<int, int>>> adj(n);
    set<pair<int, int>> used;

    auto addEdge = [&](int u, int v)
    {
        int w = rand() % 20 + 1;
        used.insert({min(u, v), max(u, v)});
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    };

    for (int i = 1; i < n; i++)
    {
        addEdge(i, rand() % i);
    }

    while ((int)used.size() < edges)
    {
        int u = rand() % n;
        int v = rand() % n;
        if (u == v || used.count({min(u, v), max(u, v)}))
        {
            continue;
        }

        addEdge(u, v);
    }

    cout << "Random graph generated" << endl;
    return adj;
}

int primsMST(int V, int src, vector<vector<pair<int, int>>> &graph)
{
    priority_queue< pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>> > pq;
    vector<bool> inMST(V, false);

    int mstcost=0;

    pq.push({0, src});

    while(pq.size()>0)
    {
        auto p = pq.top();
        int w = p.first;
        int u = p.second;
        pq.pop();

        if(inMST[u]==false)
        {
            inMST[u]=true;
            mstcost+=w;

            for(int i=0; i<graph[u].size(); i++)
            {
                int v = graph[u][i].first;
                int w = graph[u][i].second;

                pq.push({w, v});
            }
        }
    }

    return mstcost;

}



int main()
{
    srand(time(0));
    int n, edges, src;
    cout << "Enter number of vertices: ";
    cin >> n;
    cout << "Enter number of edges: ";
    cin >> edges;
    if (n <= 0 || edges < n - 1 || edges > n * (n - 1) / 2)
    {
        cout << "Invalid input: edges must be between " << max(0, n - 1)
             << " and " << n * (n - 1) / 2 << endl;
        return 1;
    }

    vector<vector<pair<int, int>>> graph = createRandomGraph(n, edges);

    cout << "Enter source node: ";
    cin >> src;
    if (src < 0 || src >= n)
    {
        cout << "Invalid source." << endl;
        return 1;
    }

    cout << "The minimum cost: " << primsMST(n,src, graph) << endl;

    return 0;
}