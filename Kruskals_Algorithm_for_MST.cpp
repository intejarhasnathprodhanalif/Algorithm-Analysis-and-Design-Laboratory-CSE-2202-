# include<bits/stdc++.h>

using namespace std;

class Edge
{
    int u, v, w;

    public:
    Edge(int u, int v, int w)
    {
        this->u=u;
        this->v=v;
        this->w=w;
    } 

    bool operator<(const Edge &other) const
    {
        return this->w < other.w;
    }

    int getU()
    {
        return u;
    }

    int getV()
    {
        return v;
    }

    int getW()
    {
        return w;
    }

};

class Graph
{
    int V;
    vector<Edge> edges;
    vector<int> parent, rank;

    public:
    Graph(int V)
    {
        this->V=V;

        for(int i=0; i<V; i++)
        {
            parent.push_back(i);
            rank.push_back(0);
        }

    }

    void addEdge(int u, int v, int w)
    {
        edges.push_back(Edge(u, v, w));
    }

    int find(int x)
    {
        if(find(x)==x)
        {
            return x;
        }

        return parent[x]=find(parent[x]);
    }

    void unionbyrank(int a, int b)
    {
        int parA=find(a);
        int parB=find(b);

        if(parA==parB)
        {
            return;
        }

        else if(rank[parA]==rank[parB])
        {
            parent[parB]=parA;
            rank[parA]++;
        }

        else if(rank[parA]>rank[parB])
        {
            parent[parB]=parA;
        }

        else
        {
            parent[parA]=parB;
        }
    }

    int Kruskal()
    {
        sort(edges.begin(), edges.end());
        int mstCost = 0;

        for(int i=0; i<edges.size(); i++)
        {
            Edge e = edges[i];

            int u = edges[i].getU();
            int v = edges[i].getV();

            int parU=find(u);
            int parV=find(v);

            if(parU!=parV)
            {
                unionbyrank(u, v);
                mstCost+=edges[i].getW();
            }
        }

        return mstCost;
    }

};

int main()
{
    return 0;
}