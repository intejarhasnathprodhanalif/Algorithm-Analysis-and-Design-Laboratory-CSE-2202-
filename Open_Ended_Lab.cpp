#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <set>
#include <map>
#include <algorithm>
#include <iomanip>
#include <limits>

using namespace std;

const double INF = numeric_limits<double>::infinity();

// ============================================================================
// 1. ENUMS & COMPATIBILITY MATRIX (Task 1 & Task 2 Constraints)
// ============================================================================
enum RoadType
{
    HIGHWAY = 0,
    URBAN = 1,
    RURAL = 2
};
enum VehicleType
{
    CAR = 0,
    TRUCK = 1,
    BIKE = 2
};

string roadTypeName(int rt)
{
    if (rt == HIGHWAY)
        return "Highway";
    if (rt == URBAN)
        return "Urban";
    return "Rural";
}

string vehicleTypeName(int vt)
{
    if (vt == CAR)
        return "Car";
    if (vt == TRUCK)
        return "Truck";
    return "Bike";
}

// Allowed[VehicleType][RoadType]
// Car: Allowed on Highway, Urban, Rural
// Truck: Allowed on Highway, Urban (Restricted on Rural)
// Bike: Allowed on Urban, Rural (Restricted on Highway)
const bool isAllowed[3][3] = {
    {true, true, true},  // CAR
    {true, true, false}, // TRUCK
    {false, true, true}  // BIKE
};

// ============================================================================
// 2. DATA STRUCTURES (Task 1 & Task 2)
// ============================================================================
struct Edge
{
    int id;
    int u, v;
    double distance; // w: physical length/distance of road
    int roadType;    // HIGHWAY, URBAN, or RURAL
    int capacity;    // Max vehicles allowed per time step
    int currentLoad; // Current number of vehicles assigned to this road
};

struct Request
{
    int id;
    int timeStep;    // Discrete time step t
    int numVehicles; // n_i
    int source;      // s_i
    int destination; // d_i
    int vehicleType; // tau_i
};

// Stores how a single request's vehicles were distributed across paths
struct PathAllocation
{
    vector<int> nodes; // Sequence of vertices from s_i to d_i
    vector<int> edges; // Sequence of edge IDs used
    int vehicleCount;  // Number of vehicles assigned to this specific path
    double physicalDistance;
};

struct RequestResult
{
    Request req;
    int routedVehicles = 0;
    int unroutedVehicles = 0; // In case network hits 100% saturation
    vector<PathAllocation> allocations;
};

struct RoadNetwork
{
    int V, E;
    vector<Edge> edges;
    vector<vector<int>> adj; // adj[u] stores edge IDs incident to u

    RoadNetwork(int vertices) : V(vertices), E(0)
    {
        adj.resize(V);
    }

    int getOtherEndpoint(int edgeId, int u) const
    {
        return (edges[edgeId].u == u) ? edges[edgeId].v : edges[edgeId].u;
    }

    // Resets road loads at the start of a new discrete time step
    void resetLoads()
    {
        for (auto &e : edges)
        {
            e.currentLoad = 0;
        }
    }
};

// ============================================================================
// 3. TASK 1: NETWORK & REQUEST GENERATORS
// ============================================================================
RoadNetwork generateRoadNetwork(int V, int E, mt19937 &rng)
{
    int maxEdges = V * (V - 1) / 2;
    E = max(V - 1, min(E, maxEdges));

    RoadNetwork net(V);
    set<pair<int, int>> existingEdges;
    uniform_int_distribution<int> roadDist(0, 2);

    auto addEdge = [&](int u, int v)
    {
        int rType = roadDist(rng);
        double dist = 0;
        int cap = 0;

        if (rType == HIGHWAY)
        {
            dist = uniform_int_distribution<int>(15, 45)(rng);
            cap = uniform_int_distribution<int>(35, 70)(rng);
        }
        else if (rType == URBAN)
        {
            dist = uniform_int_distribution<int>(5, 18)(rng);
            cap = uniform_int_distribution<int>(12, 25)(rng);
        }
        else
        { // RURAL
            dist = uniform_int_distribution<int>(8, 25)(rng);
            cap = uniform_int_distribution<int>(5, 12)(rng);
        }

        int id = net.edges.size();
        net.edges.push_back({id, u, v, dist, rType, cap, 0});
        net.adj[u].push_back(id);
        net.adj[v].push_back(id);
    };

    // Phase A: Spanning tree to guarantee a connected graph
    for (int i = 1; i < V; ++i)
    {
        int parent = uniform_int_distribution<int>(0, i - 1)(rng);
        int u = min(i, parent), v = max(i, parent);
        existingEdges.insert({u, v});
        addEdge(u, v);
    }

    // Phase B: Remaining random edges
    uniform_int_distribution<int> nodeDist(0, V - 1);
    while ((int)net.edges.size() < E)
    {
        int u = nodeDist(rng);
        int v = nodeDist(rng);
        if (u == v)
            continue;
        if (u > v)
            swap(u, v);

        if (existingEdges.find({u, v}) == existingEdges.end())
        {
            existingEdges.insert({u, v});
            addEdge(u, v);
        }
    }

    net.E = net.edges.size();
    return net;
}

vector<vector<Request>> generateRequests(int numTimeSteps, int reqPerStep, int V, mt19937 &rng)
{
    vector<vector<Request>> allSteps(numTimeSteps);
    uniform_int_distribution<int> nodeDist(0, V - 1);
    uniform_int_distribution<int> vehTypeDist(0, 2);
    uniform_int_distribution<int> vehCountDist(6, 18);

    int reqId = 0;
    for (int t = 0; t < numTimeSteps; ++t)
    {
        for (int r = 0; r < reqPerStep; ++r)
        {
            int s = nodeDist(rng);
            int d = nodeDist(rng);
            while (s == d)
                d = nodeDist(rng);

            int n_i = vehCountDist(rng);
            int tau_i = vehTypeDist(rng);
            allSteps[t].push_back({reqId++, t, n_i, s, d, tau_i});
        }
    }
    return allSteps;
}

// ============================================================================
// 4. TASK 2: CONGESTION-AWARE ROUTING ALGORITHM
// ============================================================================

// Composite Edge Cost Function: w(e) * (1 + alpha * (load / cap)^2)
double getCompositeEdgeCost(const Edge &e, double alpha)
{
    double utilization = (double)e.currentLoad / (double)e.capacity;
    return e.distance * (1.0 + alpha * utilization * utilization);
}

// Modified Dijkstra to find the single best valid path for 1 vehicle (or chunk)
bool findBestValidPath(const RoadNetwork &net, int source, int dest, int vehicleType,
                       double alpha, vector<int> &outNodes, vector<int> &outEdges,
                       double &outPhysicalDist)
{
    int V = net.V;
    vector<double> dist(V, INF);
    vector<int> parentNode(V, -1);
    vector<int> parentEdge(V, -1);

    // Min-heap storing {compositeCost, vertex}
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;

    dist[source] = 0.0;
    pq.push({0.0, source});

    while (!pq.empty())
    {
        double d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dist[u])
            continue;
        if (u == dest)
            break; // Found optimal penalized path to destination

        for (int edgeId : net.adj[u])
        {
            const Edge &e = net.edges[edgeId];

            // IDEA A: Hard Constraint 1 - Vehicle-Type Restriction
            if (!isAllowed[vehicleType][e.roadType])
                continue;

            // IDEA A: Hard Constraint 2 - Capacity Check
            if (e.currentLoad >= e.capacity)
                continue;

            int v = net.getOtherEndpoint(edgeId, u);

            // IDEA B: Dynamic Congestion-Penalized Edge Cost
            double edgeCost = getCompositeEdgeCost(e, alpha);

            if (dist[u] + edgeCost < dist[v])
            {
                dist[v] = dist[u] + edgeCost;
                parentNode[v] = u;
                parentEdge[v] = edgeId;
                pq.push({dist[v], v});
            }
        }
    }

    // If destination is unreachable due to restrictions or full capacities
    if (dist[dest] == INF)
        return false;

    // Reconstruct the path from dest back to source
    outNodes.clear();
    outEdges.clear();
    outPhysicalDist = 0.0;

    int curr = dest;
    while (curr != source)
    {
        outNodes.push_back(curr);
        int eId = parentEdge[curr];
        outEdges.push_back(eId);
        outPhysicalDist += net.edges[eId].distance;
        curr = parentNode[curr];
    }
    outNodes.push_back(source);

    reverse(outNodes.begin(), outNodes.end());
    reverse(outEdges.begin(), outEdges.end());
    return true;
}

// IDEA C: Iterative Assignment (Splits n_i vehicles across one or more paths)
RequestResult routeSingleRequest(RoadNetwork &net, const Request &req, double alpha)
{
    RequestResult result;
    result.req = req;

    // Map a sequence of nodes to its index in result.allocations so we can group identical paths
    map<vector<int>, int> pathIndexMap;

    for (int v = 0; v < req.numVehicles; ++v)
    {
        vector<int> pathNodes, pathEdges;
        double physDist = 0.0;

        bool found = findBestValidPath(net, req.source, req.destination,
                                       req.vehicleType, alpha,
                                       pathNodes, pathEdges, physDist);

        if (!found)
        {
            // No valid path with remaining capacity exists for the rest of the vehicles
            result.unroutedVehicles = req.numVehicles - v;
            break;
        }

        // Increment load on every road along the chosen path immediately
        for (int eId : pathEdges)
        {
            net.edges[eId].currentLoad += 1;
        }
        result.routedVehicles += 1;

        // Record the vehicle in the allocation list for this path
        if (pathIndexMap.find(pathNodes) == pathIndexMap.end())
        {
            int idx = result.allocations.size();
            pathIndexMap[pathNodes] = idx;
            result.allocations.push_back({pathNodes, pathEdges, 1, physDist});
        }
        else
        {
            int idx = pathIndexMap[pathNodes];
            result.allocations[idx].vehicleCount += 1;
        }
    }

    return result;
}

// Runs the routing algorithm for all requests across all discrete time steps
vector<vector<RequestResult>> runProposedSimulation(RoadNetwork &net,
                                                    const vector<vector<Request>> &allSteps,
                                                    double alpha)
{
    vector<vector<RequestResult>> simResults(allSteps.size());

    for (size_t t = 0; t < allSteps.size(); ++t)
    {
        net.resetLoads(); // Clear loads for the new discrete time step t

        for (const auto &req : allSteps[t])
        {
            RequestResult res = routeSingleRequest(net, req, alpha);
            simResults[t].push_back(res);
        }
    }
    return simResults;
}

// ============================================================================
// 5. MAIN DRIVER (Testing Task 1 & Task 2)
// ============================================================================
int main()
{
    mt19937 rng(42); // Fixed seed for reproducibility

    // Task 1: Generate a sample network (10 intersections, 18 bidirectional roads)
    RoadNetwork net = generateRoadNetwork(10, 18, rng);

    // Task 1: Generate requests across 2 discrete time steps (4 requests per step)
    vector<vector<Request>> requests = generateRequests(2, 4, net.V, rng);

    // Task 2: Run the Proposed Congestion-Aware Routing Algorithm (with alpha = 3.0)
    double alpha = 3.0;
    vector<vector<RequestResult>> results = runProposedSimulation(net, requests, alpha);

    // Quick verification output showing how Task 2 split vehicles across paths
    cout << "=== Task 1 & Task 2 Verification (V=" << net.V << ", E=" << net.E << ") ===\n";
    for (size_t t = 0; t < results.size(); ++t)
    {
        cout << "\n--- Time Step t = " << t << " ---\n";
        for (const auto &res : results[t])
        {
            cout << "Req " << res.req.id << " [" << res.req.numVehicles << "x "
                 << vehicleTypeName(res.req.vehicleType) << " from "
                 << res.req.source << " -> " << res.req.destination << "]: "
                 << "Routed = " << res.routedVehicles
                 << ", Unrouted = " << res.unroutedVehicles << "\n";

            for (const auto &alloc : res.allocations)
            {
                cout << "   -> " << setw(2) << alloc.vehicleCount << " vehicle(s) via path: ";
                for (size_t i = 0; i < alloc.nodes.size(); ++i)
                {
                    cout << alloc.nodes[i] << (i + 1 < alloc.nodes.size() ? " -> " : "");
                }
                cout << " (Physical Dist: " << alloc.physicalDistance << ")\n";
            }
        }
    }

    return 0;
}