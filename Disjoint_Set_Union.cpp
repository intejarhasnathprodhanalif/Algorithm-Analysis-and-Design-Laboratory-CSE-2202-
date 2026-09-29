#include<bits/stdc++.h>

using namespace std;

class DisjointSetUnion
{
    int n;
    vector<int> parent;
    vector<int> rank;

    public:
    DisjointSetUnion(int n)
    {
        this->n=n;
        for(int i=0; i<n; i++)
        {
            parent.push_back(i);
            rank.push_back(0);
        }
    }

    int find(int x)
    {
        if(parent[x]==x)
        {
            return x;
        }

        return parent[x]=find(parent[x]);
    }

    void unionbyrank(int a, int b)
    {
        int parA=parent[a];
        int parB=parent[b];

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

};

int main()
{
    int n;
    cout << "Enter nmuber of elements: ";
    cin >> n;
    DisjointSetUnion dsu(n);

    dsu.unionbyrank(0, 4);
    cout << "Parent of 4 is " << dsu.find(4) << endl;

    return 0;
}