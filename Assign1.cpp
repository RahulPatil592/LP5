#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

class Graph
{
    int V;
    vector<vector<int>> adj;

public:
    Graph(int n)
    {
        V = n;
        adj.resize(V);
    }

    void addEdge(int u, int v)
    {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void sequentialBFS(int source)
    {
        vector<bool> vis(V, false);
        queue<int> q;
        q.push(source);
        vis[source] = true;
        while (!q.empty())
        {
            int u = q.front();
            q.pop();

            cout << u << " ";
            for (int i = 0; i < adj[u].size(); i++)
            {
                int v = adj[u][i];
                if (!vis[v])
                {
                    q.push(v);
                    vis[v] = true;
                }
            }
        }
    }

    void sequentialDFSUtil(int u, vector<bool> &vis)
    {
        vis[u] = true;
        cout << u << " ";
        for (int i = 0; i < adj[u].size(); i++)
        {
            int v = adj[u][i];
            if (!vis[v])
            {
                sequentialDFSUtil(v, vis);
            }
        }
    }

    void sequentialDFS(int source)
    {
        vector<bool> vis(V, false);
        sequentialDFSUtil(source, vis);
    }

    void parallelBFS(int source)
    {
        vector<bool> vis(V, false);
        queue<int> q;
        q.push(source);
        vis[source] = true;
        while (!q.empty())
        {

            int u;
#pragma omp parallel shared(q, vis)
            {
#pragma omp single
                {
                    u = q.front();
                    q.pop();
                    cout << u << " ";
                }
            }
#pragma omp for
            for (int i = 0; i < adj[u].size(); i++)
            {
                int v = adj[u][i];
                if (!vis[v])
                {
#pragma omp critical
                    {
                        q.push(v);
                        vis[v] = true;
                    }
                }
            }
        }
    }

    void parallelDFSUtil(int u, vector<bool> &vis)
    {
        vis[u] = true;
        cout << u << " ";

#pragma omp parallel for
        for (int i = 0; i < adj[u].size(); i++)
        {
            int v = adj[u][i];
            if (!vis[v])
            {
#pragma omp task
                parallelDFSUtil(v, vis);
            }
        }
    }

    void parallelDFS(int source)
    {

        vector<bool> vis(V, false);
#pragma omp parallel
        {
// Single thread starts the DFS
#pragma omp single
            {
                parallelDFSUtil(source, vis);
            }

// Ensure all tasks finish before exiting the parallel region
#pragma omp taskwait
        }
    }
};

int main()
{
    omp_set_num_threads(4);
    int num_vertices, num_edges, source;
    cout << "Enter the number of vertices : ";
    cin >> num_vertices;
    Graph g(num_vertices);

    cout << "Enter the number of edges : ";
    cin >> num_edges;

    cout << "Enter the edges (u,v) : " << endl;
    for (int i = 0; i < num_edges; i++)
    {
        int u, v;
        cin >> u >> v;
        g.addEdge(u, v);
    }

    cout << "Enter the starting source point for DFS and BFS : ";
    cin >> source;

    cout << "\nNumber of threads used : " << omp_get_max_threads() << endl;

    // sequential BFS
    cout << "Sequential BFS starting from vertex : " << source << endl;
    double start = omp_get_wtime();
    g.sequentialBFS(source);
    double stop = omp_get_wtime();
    cout << endl;
    double duration = stop - start;
    cout << "Time taken by sequential BFS : " << duration * 1e6 << "microseconds" << endl;

    // sequential DFS
    cout << "Sequential DFS starting from vertex : " << source << endl;
    start = omp_get_wtime();
    g.sequentialDFS(source);
    stop = omp_get_wtime();
    cout << endl;
    duration = stop - start;
    cout << "Time taken by sequential DFS : " << duration * 1e6 << "microseconds" << endl;

    // parallel DFS
    cout << "Parallel BFS starting from vertex : " << source << endl;
    start = omp_get_wtime();
    g.parallelBFS(source);
    stop = omp_get_wtime();
    cout << endl;
    duration = stop - start;
    cout << "Time taken by Parallel BFS : " << duration * 1e6 << "microseconds" << endl;

    // parallel DFS
    cout << "Parallel DFS starting from vertex : " << source << endl;
    start = omp_get_wtime();
    g.parallelDFS(source);
    stop = omp_get_wtime();
    cout << endl;
    duration = stop - start;
    cout << "Time taken by Parallel DFS : " << duration * 1e6 << "microseconds" << endl;
    return 0;
}