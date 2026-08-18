#include<bits/stdc++.h>

using namespace std;

void addEdge(vector<vector<pair<int, int>>> &graph, int u, int v, int w)
{
    graph[u].push_back({v, w});
}

void printGraph(vector<vector<pair<int, int>>> &graph)
{
    int V=graph.size();

    for(int u=0; u<V; u++)
    {
        cout << u << ": ";

        for(auto edge:graph[u])
        {
            cout << "(" << edge.first << ", w=" << edge.second << ") ";
        }

        cout << endl;
    }
}

void Dijkstra(vector<vector<pair<int, int>>> &graph, int src)
{
    int V=graph.size();

    vector<int> dist(V, INT_MAX);
    dist[src]=0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, src});

    while (!pq.empty())
    {
        int d = pq.top().first;
        int u = pq.top().second;

        pq.pop();

        // If the distance in the priority queue is greater than the current distance, skip
        if (d > dist[u])
        {
            continue;
        }

        // Iterate through all adjacent vertices of u
        for (auto edge : graph[u])
        {
            int v = edge.first;
            int w = edge.second;

            // Edge relaxation
            if (dist[v] > dist[u] + w)
            {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }

    cout << "Shortest distances from vertex " << src << ":" << endl;
    for (int i = 0; i < V; i++)
    {
        if (dist[i] == INT_MAX)
            cout << i << ": unreachable" << endl;
        else
            cout << i << ": " << dist[i] << endl;
    }
}

    int main()
{
    int V, E;
    cout << "Enter number of vertices: ";
    cin >> V;
    cout << endl;

    cout << "Enter number of edges: ";
    cin >> E;
    cout << endl;

    vector<vector<pair<int, int>>> graph(V);

    int u, v, w;
    cout << "Enter source destination weight: ";

    for(int i=0; i<E; i++)
    {
        cin >> u >> v >> w;
        cout << endl;

        addEdge(graph, u, v, w);
    }

    cout << endl;
    printGraph(graph);

    cout << "Shortest Path" << endl;

    int src;
    cout << "Enter source: ";
    cin >> src;

    Dijkstra(graph, src);
    cout << endl;

    return 0;
}

//graph -> outer vector
//graph[u] -> inner vector
//graph[u][i] -> pair<int,int> (v,w) where v is the adjacent vertex and w is the weight of the edge u->v
//graph[u][i].first -> v(vertex)
//graph[u][i].second -> w(weight)
//"edge" in the for loop in Dijkstra function is a pair<int,int> (v,w) where v is the adjacent vertex and w is the weight of the edge u->v