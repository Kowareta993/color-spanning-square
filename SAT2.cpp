#include "SAT2.h"
#include <list>
#include <stack>
#include <iostream>

typedef struct {
    list<int> *adj;
} Graph;

Graph graph, invGraph;
int *SCC;

int index(int i) {
    if (i < 0)
        return -i * 2 - 1;
    return i * 2 - 2;
}


void createGraphs(int *a, int *b, int n, int m) {
    graph = Graph{};
    graph.adj = new list<int>[2 * n];
    invGraph = Graph{};
    invGraph.adj = new list<int>[2 * n];
    for (int i = 0; i < m; i++) {
        graph.adj[index(-a[i])].push_back(index(b[i]));
        graph.adj[index(-b[i])].push_back(index(a[i]));
        invGraph.adj[index(b[i])].push_back(index(-a[i]));
        invGraph.adj[index(a[i])].push_back(index(-b[i]));
    }
}

int CC;

void dfs(Graph G, int node, bool *visited, stack<int> &stack) {
    stack.push(node);
    SCC[node] = CC;
    visited[node] = true;
    for (auto it = G.adj[node].begin(); it != G.adj[node].end(); it++) {
        if (!visited[*it])
            dfs(G, *it, visited, stack);
    }
}


void findSCCs(int n) {
    stack<int> s1;
    bool *visited = new bool[n];
    for (int i = 0; i < n; ++i) {
        visited[i] = false;
    }
    for (int i = 0; i < n; ++i) {
        if (!visited[i])
            dfs(graph, i, visited, s1);
    }
    stack<int> stack2;
    for (int i = 0; i < n; ++i) {
        stack2.push(s1.top());
        s1.pop();
    }
    for (int i = 0; i < n; ++i) {
        visited[i] = false;
        SCC[i] = -1;
    }
    CC = 0;
    while (!stack2.empty()) {
        if (visited[stack2.top()]) {
            stack2.pop();
            continue;
        }
        dfs(invGraph, stack2.top(), visited, s1);
        stack2.pop();
        CC++;
    }
    free(visited);
}

void deallocate() {
    free(SCC);
//    free(graph.adj);
//    free(invGraph.adj);
}

bool satisfiable(int *a, int *b, int n, int m) {
    createGraphs(a, b, n, m);
    SCC = new int[2 * n];
    findSCCs(2 * n);
    for (int i = 1; i <= n; ++i) {
        if (SCC[index(i)] == SCC[index(-i)]) {
            deallocate();
            return false;
        }
    }
    deallocate();
    return true;
}