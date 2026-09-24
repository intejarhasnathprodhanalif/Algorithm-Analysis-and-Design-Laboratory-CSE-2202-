#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll INF = LLONG_MAX / 4;

inline ll addW(ll a, ll b)
{
    if (a >= INF || b >= INF)
        return INF;
    return a + b;
}

// ============================================================================
//  The main data structure
// ============================================================================
class DynamicAPSP
{
private:
    int n;
    vector<map<int, ll>> out, in;
    vector<vector<ll>> D;
    vector<char> inS;

    vector<ll> dijkstra(int s) const
    {
        vector<ll> dist(n, INF);
        priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
        dist[s] = 0;
        pq.push({0, s});
        while (!pq.empty())
        {
            pair<ll, int> top = pq.top();
            pq.pop();
            ll d = top.first;
            int x = top.second;
            if (d > dist[x])
                continue;
            for (const auto &e : out[x])
            {
                int y = e.first;
                ll nd = d + e.second;
                if (nd < dist[y])
                {
                    dist[y] = nd;
                    pq.push({nd, y});
                }
            }
        }
        return dist;
    }

    void goodNewsUpdate(int a, int b, ll w)
    {
        vector<ll> colA(n), rowB(n);
        for (int i = 0; i < n; i++)
        {
            colA[i] = D[i][a];
            rowB[i] = D[b][i];
        }

        vector<int> srcs;
        for (int u = 0; u < n; u++)
            if (colA[u] < INF && colA[u] + w < D[u][b])
                srcs.push_back(u);

        vector<int> dsts;
        for (int v = 0; v < n; v++)
            if (rowB[v] < INF && w + rowB[v] < D[a][v])
                dsts.push_back(v);

        for (int u : srcs)
        {
            ll base = colA[u] + w;
            for (int v : dsts)
            {
                ll cand = base + rowB[v];
                if (cand < D[u][v])
                {
                    D[u][v] = cand;
                    pairsChanged++;
                }
            }
        }
    }

    void badNewsUpdate(int a, int b, ll oldW)
    {
        for (int u = 0; u < n; u++)
        {
            if (u == b)
                continue;
            if (D[u][a] >= INF)
                continue;
            if (addW(D[u][a], oldW) != D[u][b])
                continue;

            repairOneSource(u, b);
            sourcesRepaired++;
        }
    }

    void repairOneSource(int u, int b)
    {
        vector<ll> &d = D[u];

        vector<int> cand;
        cand.push_back(b);
        inS[b] = 1;
        for (size_t h = 0; h < cand.size(); h++)
        {
            int y = cand[h];
            for (const auto &e : out[y])
            {
                int z = e.first;
                if (!inS[z] && z != u && d[y] + e.second == d[z])
                {
                    inS[z] = 1;
                    cand.push_back(z);
                }
            }
        }

        vector<int> freed;
        for (int z : cand)
        {
            if (inS[z] && hasOutsideSupport(z, d))
            {
                inS[z] = 0;
                freed.push_back(z);
            }
        }
        for (size_t h = 0; h < freed.size(); h++)
        {
            int y = freed[h];
            for (const auto &e : out[y])
            {
                int z = e.first;
                if (inS[z] && d[y] + e.second == d[z])
                {
                    inS[z] = 0;
                    freed.push_back(z);
                }
            }
        }

        priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
        for (int z : cand)
        {
            if (!inS[z])
                continue;
            verticesRepaired++;
            ll best = INF;
            for (const auto &e : in[z])
            {
                int y = e.first;
                if (!inS[y] && d[y] < INF)
                    best = min(best, d[y] + e.second);
            }
            d[z] = best;
            if (best < INF)
                pq.push({best, z});
        }
        while (!pq.empty())
        {
            pair<ll, int> top = pq.top();
            pq.pop();
            ll dz = top.first;
            int z = top.second;
            if (dz > d[z])
                continue;
            for (const auto &e : out[z])
            {
                int c = e.first;
                if (inS[c] && dz + e.second < d[c])
                {
                    d[c] = dz + e.second;
                    pq.push({d[c], c});
                }
            }
        }

        for (int z : cand)
            inS[z] = 0;
    }

    bool hasOutsideSupport(int z, const vector<ll> &d) const
    {
        for (const auto &e : in[z])
        {
            int y = e.first;
            if (!inS[y] && d[y] < INF && d[y] + e.second == d[z])
                return true;
        }
        return false;
    }

public:
    long long pairsChanged = 0;
    long long sourcesRepaired = 0;
    long long verticesRepaired = 0;

    explicit DynamicAPSP(int n_) : n(n_), out(n_), in(n_),
                                   D(n_, vector<ll>(n_, INF)), inS(n_, 0)
    {
        for (int i = 0; i < n; i++)
            D[i][i] = 0;
    }

    int size() const { return n; }

    void addEdgeInitial(int a, int b, ll w)
    {
        checkVertex(a);
        checkVertex(b);
        if (w < 0)
            throw invalid_argument("weights must be non-negative");
        if (a == b)
            return;
        out[a][b] = w;
        in[b][a] = w;
    }

    void build()
    {
        for (int s = 0; s < n; s++)
            D[s] = dijkstra(s);
    }

    ll query(int u, int v) const { return D[u][v]; }

    void insertEdge(int a, int b, ll w)
    {
        checkVertex(a);
        checkVertex(b);
        if (w < 0)
            throw invalid_argument("weights must be non-negative");
        if (a == b)
            return;
        if (out[a].count(b))
        {
            setWeight(a, b, w);
            return;
        }
        out[a][b] = w;
        in[b][a] = w;
        goodNewsUpdate(a, b, w);
    }

    void deleteEdge(int a, int b)
    {
        checkVertex(a);
        checkVertex(b);
        auto it = out[a].find(b);
        if (it == out[a].end())
            return;
        ll oldW = it->second;
        out[a].erase(it);
        in[b].erase(a);
        badNewsUpdate(a, b, oldW);
    }

    void setWeight(int a, int b, ll w)
    {
        checkVertex(a);
        checkVertex(b);
        if (w < 0)
            throw invalid_argument("weights must be non-negative");
        auto it = out[a].find(b);
        if (it == out[a].end())
        {
            insertEdge(a, b, w);
            return;
        }
        ll oldW = it->second;
        if (w == oldW)
            return;
        it->second = w;
        in[b][a] = w;
        if (w < oldW)
            goodNewsUpdate(a, b, w);
        else
            badNewsUpdate(a, b, oldW);
    }

    vector<vector<ll>> recomputeFromScratch() const
    {
        vector<vector<ll>> M(n);
        for (int s = 0; s < n; s++)
            M[s] = dijkstra(s);
        return M;
    }

    bool matchesScratch() const { return D == recomputeFromScratch(); }

    void printMatrix(ostream &os) const
    {
        for (int u = 0; u < n; u++)
        {
            for (int v = 0; v < n; v++)
            {
                if (v)
                    os << ' ';
                if (D[u][v] >= INF)
                    os << setw(4) << "INF";
                else
                    os << setw(4) << D[u][v];
            }
            os << '\n';
        }
    }

    void checkVertex(int x) const
    {
        if (x < 0 || x >= n)
            throw out_of_range("vertex id out of range");
    }
};

// ============================================================================
//  Interactive Mode
// ============================================================================

void runInteractive()
{
    int n, m;
    cout << "=== Dynamic APSP Setup ===\n";
    cout << "Enter number of vertices (n): ";
    cin >> n;
    cout << "Enter number of initial edges (m): ";
    cin >> m;

    DynamicAPSP g(n);

    if (m > 0)
    {
        cout << "\n--- Initializing Edges ---\n";
        for (int i = 0; i < m; i++)
        {
            int a, b;
            ll w;
            cout << "Edge " << i + 1 << " - Enter source vertex: ";
            cin >> a;
            cout << "Edge " << i + 1 << " - Enter destination vertex: ";
            cin >> b;
            cout << "Edge " << i + 1 << " - Enter weight: ";
            cin >> w;
            g.addEdgeInitial(a, b, w);
        }
    }
    g.build();
    cout << "\n[Output] Initial graph built successfully.\n";

    while (true)
    {
        cout << "\n=== Operations Menu ===\n";
        cout << "1. Insert/Update Edge (INS / SET)\n";
        cout << "2. Delete Edge (DEL)\n";
        cout << "3. Query Distance (QRY)\n";
        cout << "4. Print Distance Matrix (PRINT)\n";
        cout << "5. Check Integrity (CHECK)\n";
        cout << "6. Exit\n";
        cout << "Enter choice (1-6): ";

        int choice;
        if (!(cin >> choice))
        {
            // Clear the error flag and ignore the bad input to prevent infinite loops
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "[Error Output]: Invalid input. Please enter a number.\n";
            continue;
        }

        try
        {
            if (choice == 1)
            {
                int a, b;
                ll w;
                cout << "Enter source vertex: ";
                cin >> a;
                cout << "Enter destination vertex: ";
                cin >> b;
                cout << "Enter new weight: ";
                cin >> w;
                g.insertEdge(a, b, w);
                cout << "[Output] Edge " << a << "->" << b << " successfully updated with weight " << w << ".\n";
            }
            else if (choice == 2)
            {
                int a, b;
                cout << "Enter source vertex: ";
                cin >> a;
                cout << "Enter destination vertex: ";
                cin >> b;
                g.deleteEdge(a, b);
                cout << "[Output] Edge " << a << "->" << b << " successfully deleted.\n";
            }
            else if (choice == 3)
            {
                int u, v;
                cout << "Enter source vertex: ";
                cin >> u;
                cout << "Enter destination vertex: ";
                cin >> v;
                g.checkVertex(u);
                g.checkVertex(v);
                ll d = g.query(u, v);

                cout << "[Output] Shortest Distance (" << u << " -> " << v << ") = ";
                if (d >= INF)
                    cout << "INF\n";
                else
                    cout << d << "\n";
            }
            else if (choice == 4)
            {
                cout << "\n[Output] Current Distance Matrix:\n";
                g.printMatrix(cout);
            }
            else if (choice == 5)
            {
                cout << "[Output] Integrity Check Result: ";
                cout << (g.matchesScratch() ? "OK - Matrix matches full recomputation.\n"
                                            : "MISMATCH! Matrix integrity compromised.\n");
            }
            else if (choice == 6)
            {
                cout << "[Output] Exiting program.\n";
                break;
            }
            else
            {
                cout << "[Output] Invalid choice. Please enter a number between 1 and 6.\n";
            }
        }
        catch (const exception &ex)
        {
            cout << "[Error Output]: " << ex.what() << "\n";
        }
    }
}

int main()
{
    // Fast I/O is removed so cout flushes immediately before cin
    runInteractive();
    return 0;
}