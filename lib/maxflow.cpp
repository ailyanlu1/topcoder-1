#include <vector>
#include <queue>
#include <cassert>
#include <iostream>

using namespace std;

/*
 
 Max flow algorithm Dinic, O(E*V^2)
 
 Usage:
  // Don't miss updating MAX_V constant
  
  // initialize
  MaxFlow_dinic<int> f;
 
  // configure edges with capacity
  f.addEdge(u0,v0,c0);
  f.addEdge(u1,v1,c1);
 
  // compute flow
  flow = f.maxFlow(source, sink)
 
 */

#define MAX_V 100

template<class V> class MaxFlow_dinic {
public:
  void addEdge(int from, int to, V cap) {
    E[from].push_back((Edge){  to, (int)E[  to].size()  , cap});
    E[  to].push_back((Edge){from, (int)E[from].size()-1,   0}); // residual graph
  }
  
  V maxFlow(int s, int t) {
    V flow = 0;
    while (true) {
      bfs(s);
      if (level[t] < 0) return flow; // t is unreachable
      
      memset(iter, 0, sizeof(iter));
      int f;
      while ((f = dfs(s, t, numeric_limits<V>::max())) > 0) {
        flow += f;
      }
    }
    
    return flow;
  }
private:
  struct Edge { int to, rev; V cap; };
  vector<Edge> E[MAX_V]; // Graph
  int level[MAX_V]; // distance from `s`
  int iter[MAX_V];
  
  // initialize shoftest path and store in `level`
  void bfs(int s) {
    memset(level, -1, sizeof(level));
    
    queue<int> Q;
    level[s] = 0;
    Q.push(s);
    
    while (!Q.empty()) {
      int u = Q.front(); Q.pop();
      
      for (int i = 0; i < E[u].size(); i++) {
        Edge &e = E[u][i];
        int v = e.to;
        if (e.cap<=0) continue;
        if (level[v]>=0) continue; // visited
        
        level[v] = level[u] + 1;
        Q.push(v);
      }
    }
  }
  
  // find augmenting path in residual network and update f
  int dfs(int u, int t, V f) {
    if (u == t) return f;
    for (int &i = iter[u]; i < E[u].size(); i++) {
      Edge &e = E[u][i];
      V v = e.to;
      if (e.cap <= 0) continue;
      if (level[u] >= level[v]) continue;
      
      int d = dfs(v, t, min(f, e.cap));
      if (d <= 0) continue;
      e.cap -= d;
      E[v][e.rev].cap += d;
      return d;
    }
    
    return 0;
  }
};

int main(int argc, char const *argv[]) {  
  // CLRS Figure 26.6
  vector<vector<int>> C = {
    {   0,  12,  13,   0,   0,   0 },
    {   0,   0,   0,  12,   0,   0 },
    {   0,   4,   0,   0,  14,   0 },
    {   0,   0,   9,   0,   0,  20 },
    {   0,   0,   0,   7,   0,   4 },
    {   0,   0,   0,   0,   0,   0 }
  };
  
  MaxFlow_dinic<int> f;
  for(int u=0; u<C.size(); ++u) {
    for(int v=0; v<C.size(); ++v) {
      if (C[u][v]==0) continue;
      f.addEdge(u,v,C[u][v]);
    }
  }
  int maxf = f.maxFlow(0,C.size()-1);
  assert(maxf == 23);
}
