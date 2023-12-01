#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>

using namespace std;

struct Edge
{
    int u, v, cost;
    bool build;  // true if it's build, false if it's destroy
};

struct UnionFind
{
    vector<int> parent, rank;

    UnionFind(int n) : parent(n), rank(n, 0)
    {
        for (int i = 0; i < n; i++) parent[i] = i;
    }

    int find(int x)
    {
        if (x != parent[x]) parent[x] = find(parent[x]);
        return parent[x];
    }

    bool unite(int x, int y)
    {
        x = find(x);
        y = find(y);
        if (x == y) return false;
        if (rank[x] < rank[y]) swap(x, y);
        parent[y] = x;
        if (rank[x] == rank[y]) rank[x]++;
        return true;
    }
};

int Cost(char c)
{
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    return -1;
}

bool isConnected(const vector<Edge>& edges, int numVertices)
{
    UnionFind uf(numVertices);

    for (const Edge& edge : edges)
        if (!edge.build) uf.unite(edge.u, edge.v);

    int numComponents = 0;
    for (int i = 0; i < numVertices; ++i)
        if (uf.find(i) == i) numComponents++;

    return numComponents == 1; // The graph is connected if it has only one component
}


int main()
{
    // input
    string country, build, destroy;
    cin >> country >> build >> destroy;

    vector<string> C, B, D;
    size_t pos = 0;
    while ((pos = country.find(',')) != string::npos)
    {
        C.push_back(country.substr(0, pos));
        country.erase(0, pos + 1);
    }
    C.push_back(country);

    pos = 0;
    while ((pos = build.find(',')) != string::npos)
    {
        B.push_back(build.substr(0, pos));
        build.erase(0, pos + 1);
    }
    B.push_back(build);

    pos = 0;
    while ((pos = destroy.find(',')) != string::npos) {
        D.push_back(destroy.substr(0, pos));
        destroy.erase(0, pos + 1);
    }
    D.push_back(destroy);

    int n = C.size();

    // edges
    vector<Edge> edges;
    for (int i = 0; i < n; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            if (C[i][j] == '1') edges.push_back({ i, j, Cost(D[i][j]), false });
            else edges.push_back({ i, j, Cost(B[i][j]), true });
        }
    }
    int currentEdges = count_if(edges.begin(), edges.end(), [](const Edge& e) { return !e.build; });

    // build edge & destroy edge
    vector<Edge> buildEdges, destroyEdges;
    for (const Edge& e : edges)
    {
        if (e.build) buildEdges.push_back(e);
        else destroyEdges.push_back(e);
    }

    // sort
    auto costComparator = [](const Edge& e1, const Edge& e2) { return e1.cost < e2.cost; };
    sort(buildEdges.begin(), buildEdges.end(), costComparator);
    sort(destroyEdges.begin(), destroyEdges.end(), costComparator);

    int total_cost = 0;
    UnionFind uf(n);

    set<int> representatives;
    for (const Edge& e : edges)
        if (e.build == false) uf.unite(e.u, e.v);
    for (int i = 0; i < n; ++i)
        representatives.insert(uf.find(i));

    vector<Edge> builtEdges, destroyedEdges;

    // first build edges
    if (!isConnected(edges, n))
    {
        for (const Edge& e : buildEdges)
        {
            if (uf.find(e.u) != uf.find(e.v))
            {
                uf.unite(e.u, e.v);
                total_cost += e.cost;
                builtEdges.push_back(e);

                auto it = find_if(edges.begin(), edges.end(), [&](const Edge& edge) {
                    return edge.u == e.u && edge.v == e.v;
                    });
                if (it != edges.end()) it->build = false;


                currentEdges++;
                if (isConnected(edges, n)) break;
            }
        }
    }

    // second destroy edges
    for (const Edge& e : destroyEdges)
    {
        if (currentEdges <= n - 1) break;

        // Backup the current state of the union find
        vector<int> backupParent = uf.parent;
        vector<int> backupRank = uf.rank;

        uf.unite(e.u, e.v); // Try to destroy the edge
        auto it = find_if(edges.begin(), edges.end(), [&](const Edge& edge) {
            return edge.u == e.u && edge.v == e.v && !edge.build;
            });
        if (it != edges.end()) it->build = true;

        if (isConnected(edges, n))
        {
            total_cost += e.cost;
            destroyedEdges.push_back(e);
            currentEdges--;
        }
        else
        {
            // Restore the state of the union find if the edge cannot be destroyed
            uf.parent = backupParent;
            uf.rank = backupRank;
            auto it = find_if(edges.begin(), edges.end(), [&](const Edge& edge) {
                return edge.u == e.u && edge.v == e.v;
                });
            if (it != edges.end()) it->build = false;
        }
    }

    cout << total_cost << endl;

    return 0;
}