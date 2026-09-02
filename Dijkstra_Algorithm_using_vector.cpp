#include<bits/stdc++.h>

using namespace std;

int INF=1e9;

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

    vector<int> dist(V, INF);
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
        if (dist[i] == INF)
            cout << i << ": unreachable" << endl;
        else
            cout << i << ": " << dist[i] << endl;
    }
}

void showPath(vector<vector<pair<int, int>>> &graph, int src, int dest)
{
    int V = graph.size();

    vector<int> dist(V, INT_MAX);
    vector<int> parent(V, -1); // Tracks the predecessor of each vertex
    dist[src] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, src});

    while (!pq.empty())
    {
        int d = pq.top().first;
        int u = pq.top().second;

        pq.pop();

        if (d > dist[u])
            continue;

        // Optimization: Stop algorithm early if destination is fully processed
        if (u == dest)
            break;

        for (auto edge : graph[u])
        {
            int v = edge.first;
            int w = edge.second;

            if (dist[v] > dist[u] + w)
            {
                dist[v] = dist[u] + w;
                parent[v] = u; // Record that we reached 'v' from 'u'
                pq.push({dist[v], v});
            }
        }
    }

    // Path Reconstruction
    if (dist[dest] == INT_MAX)
    {
        cout << "Vertex " << dest << " is unreachable from " << src << endl;
        return;
    }

    // Trace backwards from destination using the parent array
    vector<int> path;
    for (int curr = dest; curr != -1; curr = parent[curr])
    {
        path.push_back(curr);
    }

    cout << "Shortest distance from " << src << " to " << dest << ": " << dist[dest] << endl;
    cout << "Path: ";

    // Print the path in reverse order (from start to end)
    for (int i = path.size() - 1; i >= 0; i--)
    {
        cout << path[i];
        if (i > 0)
            cout << " -> ";
    }
    cout << endl;
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

    cout << "For Shortest path from a source to destination" << endl;
    int dest;
    cout << "Enter source: ";
    cin >> src;
    cout << endl;
    cout << "Enter destination: ";
    cin >> dest;
    cout << endl;

    showPath(graph, src, dest);

    return 0;
}

//graph -> outer vector
//graph[u] -> inner vector
//graph[u][i] -> pair<int,int> (v,w) where v is the adjacent vertex and w is the weight of the edge u->v
//graph[u][i].first -> v(vertex)
//graph[u][i].second -> w(weight)
//"edge" in the for loop in Dijkstra function is a pair<int,int> (v,w) where v is the adjacent vertex and w is the weight of the edge u->v