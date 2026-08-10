#include <bits/stdc++.h>
using namespace std;

class Graph //Graph Implementation using adjacency list
{
    int V;
    list<pair<int, int>> *l;

public:
    Graph(int v)
    {
        this->V = v;
        l = new list<pair<int, int>>[V];
    }

    void addEdge(int u, int v, int w)
    {
        l[u].push_back({v, w});
    }

    void printGraph()
    {
        for (int u = 0; u < V; u++)
        {
            cout << u << ": ";
            for (auto edge : l[u])
            {
                cout << "(" << edge.first << ", w=" << edge.second << ") ";
            }
            cout << endl;
        }
    }

    void dijkstra(int src) //function to implement Dijkstra's algorithm to get the shortest path from source vertex to all other vertices
    {
        vector<int> dist(V, INT_MAX); // Initialize distances to all vertices as infinite
                                      // vector to keep track of the shortest distance from the source vertex to each vertex
        dist[src] = 0;

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; // Min-heap priority queue to store (distance, vertex) pairs
        pq.push({0, src}); // Push the source vertex with distance 0 into the priority queue

        while (!pq.empty())
        {

            int d = pq.top().first; // Get the vertex with the minimum distance from the priority queue
            int u = pq.top().second; // Get the vertex with the minimum distance from the priority queue

            pq.pop();

            if (d > dist[u]) // If the distance in the priority queue is greater than the current distance, skip processing this vertex
            {
                continue;
            }

            for (auto edge : l[u]) // Iterate through all adjacent vertices of u
            {
                int v = edge.first; // Get the adjacent vertex
                int w = edge.second; // Get the weight of the edge u->v

                if (dist[v] > dist[u] + w) // If the distance to vertex v can be shortened by taking the edge u->v(edge relaxation)
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
};

int main()
{
    int V, u, v, w;

    cout << "Number of vertices: ";
    cin >> V;
    cout << endl;
    Graph g(V);

    cout << "Enter edge as source destination weight" << endl;
    cout << "Enter -1 -1 -1 to stop" << endl;

    while (true) // Loop to take input for edges until the user enters -1 -1 -1
    {
        cin >> u >> v >> w;

        if (u == -1)
        {
            break;
        }

        g.addEdge(u, v, w); // Add the edge to the graph
    }

    cout << endl;
    g.printGraph();

    int src;
    cout << "Enter source vertex for Dijkstra: ";
    cin >> src;

    g.dijkstra(src); // Call the dijkstra function to find the shortest paths from the source vertex

    return 0;
}