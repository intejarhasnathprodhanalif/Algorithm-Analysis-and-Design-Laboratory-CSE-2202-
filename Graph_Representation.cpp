#include<bits/stdc++.h>

using namespace std;

int main()
{
    /*Different Graph Representations*/

    /*Adjacency List*/
    /*
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
    */

    /*Edge List*/
    /*
    #include <iostream>
#include <vector>

using namespace std;

// Structure to represent a single edge
struct Edge {
    int u;
    int v;
    int weight;
};

class Graph {
private:
    vector<Edge> edges;

public:
    // Function to add a directed edge to the edge list
    void addEdge(int u, int v, int weight = 0) {
        edges.push_back({u, v, weight});
    }

    // Function to print the graph
    void printGraph() {
        if (edges.empty()) {
            cout << "Graph is empty.\n";
            return;
        }

        for (const auto& edge : edges) {
            cout << "Edge: " << edge.u << " -> " << edge.v
                 << " (Weight: " << edge.weight << ")\n";
        }
    }
};

int main() {
    Graph g;

    // Adding edges (u, v, weight)
    g.addEdge(0, 1, 4);
    g.addEdge(0, 7, 8);
    g.addEdge(1, 2, 8);
    g.addEdge(1, 7, 11);

    cout << "Edge List Representation:\n";
    g.printGraph();

    return 0;
}
    */

    return 0;
}