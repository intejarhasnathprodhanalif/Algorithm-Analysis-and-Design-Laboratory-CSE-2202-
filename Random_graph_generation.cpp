#include<bits/stdc++.h>

using namespace std;

//random graph generation
vector<vector<pair<int, int>>> createRandomGraph(int n, int edges)
{
    vector<vector<pair<int, int>>> adj(n);
    set<pair<int, int>> used;

    auto addEdge = [&] (int u, int v)
    {
        int w = rand() % 20 + 1;
        used.insert({min(u, v), max(u, v)});
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    };

    for(int i=1; i<n; i++)
    {
        addEdge(i, rand()%i);
    }

    while((int)used.size() < edges)
    {
        int u = rand() % n;
        int v = rand() % n;
        if (u==v || used.count({min(u, v), max(u, v)}))
        {
            continue;
        }

        addEdge(u, v);
    }

    return adj;
}

//print graph
void printGraph(vector<vector<pair<int, int>>> &graph)
{
    for(int u=0; u<graph.size(); u++)
    {
        cout << "Vertex " << u << "->";

        if(graph[u].empty())
        {
            cout << "No outgoing edges" << endl;
        }

        else
        {
            for(auto &edges : graph[u])
            {
                int v = edges.first;
                int w = edges.second;

                cout << "[Node " << v << ", Weight " << w << " ]";
            }
        }

        cout << endl;

    }
}

int main()
{
    int n, edges;
    cout << "Enter number of vertices: ";
    cin >> n;
    cout << "Enter number of edges: ";
    cin >> edges;

    vector<vector<pair<int, int>>> graph = createRandomGraph(n, edges);
    printGraph(graph);

    return 0;
}