#include<bits/stdc++.h>

using namespace std;

void addEdge(vector<vector<pair<int, int>>> &graph, int u, int v, int w)
{
    graph[u].push_back({v,w});
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

void FloydWarshall(vector<vector<pair<int, int>>> &graph)
{
    int INF=1e9; //A large value to represent infinity
    int V=graph.size();
    vector<vector<int>> dist(V, vector<int>(V, INF));

    for(int u=0; u<V; u++)
    {
        dist[u][u] = 0; //Distance from a vertex to itsdelf is 0

        for(auto edge : graph[u])
        {
            int v=edge.first;
            int w=edge.second;
            dist[u][v]=w; //set the weight of the edge from u to v
        }
    }

    //shortest path determination
    for(int k=0; k<V; k++)
    {
        for(int i=0; i<V; i++)
        {
            for(int j=0; j<V; j++)
            {
                dist[i][j]=min(dist[i][j], (dist[i][k]+dist[k][j]));
            }
        }
    }

    //print the shortest path
    cout << "Shortest path matrix: " << endl;
    for(int i=0; i<V; i++)
    {
        for(int j=0; j<V; j++)
        {
            if(dist[i][j]==INF)
            {
                cout << "INF ";
            }

            else
            {
                cout << dist[i][j] << " ";
            }
        }
        cout << endl;   
    }
}

int main()
{
    int V, E;
    cout << "Enter the number of vertices: ";
    cin >> V;
    cout << "Enter the number of edges: ";
    cin >> E;

    vector<vector<pair<int, int>>> graph(V);

    int u, v, w;

    for(int i=0; i<E; i++)
    {
        cout << "Enter source: ";
        cin >> u;
        cout << "Enter destination: ";
        cin >> v;
        cout << "Enter weight: ";
        cin >> w;
        cout << endl;
        addEdge(graph, u, v, w);
    }

    printGraph(graph);

    cout << "Shortest path from every source to every node: " << endl;

    FloydWarshall(graph);

    return 0;
}