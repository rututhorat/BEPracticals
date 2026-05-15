#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

class Graph {
    int V;
    vector<vector<int>> adj;
    vector<bool> visited;

public:
    Graph(int V) {
        this->V = V;
        adj.resize(V);
        visited.resize(V, false);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void parallelDFSUtil(int node) {
        bool alreadyVisited;

        // Check + mark inside critical
        #pragma omp critical
        {
            alreadyVisited = visited[node];
            if (!visited[node]) {
                visited[node] = true;
                cout << node << " ";
            }
        }

        // If already visited, exit OUTSIDE critical
        if (alreadyVisited) return;

        #pragma omp parallel for
        for (int i = 0; i < adj[node].size(); i++) {
            int neighbor = adj[node][i];
            if (!visited[neighbor]) {
                parallelDFSUtil(neighbor);
            }
        }
    }

    void parallelDFS(int start) {
        cout << "Parallel DFS Traversal: ";
        parallelDFSUtil(start);
        cout << endl;
    }
};

int main() {
    int V, E;
    cout << "Enter number of vertices: ";
    cin >> V;

    cout << "Enter number of edges: ";
    cin >> E;

    Graph g(V);

    cout << "Enter edges (u v):\n";
    for (int i = 0; i < E; i++) {
        int u, v;
        cin >> u >> v;
        g.addEdge(u, v);
    }

    int start;
    cout << "Enter starting vertex: ";
    cin >> start;

    g.parallelDFS(start);

    return 0;
}

/* output
PS C:\Users\rutuj\Desktop\Practicals\HPCpractical> g++ dfs.cpp -fopenmp -o dfs
PS C:\Users\rutuj\Desktop\Practicals\HPCpractical> .\dfs.exe
Enter number of vertices: 6
Enter number of edges: 5
Enter edges (u v):
0 1
0 2
1 3 
1 4
2 5
Enter starting vertex: 0
Parallel DFS Traversal: 0 1 2 3 5 4 


*/