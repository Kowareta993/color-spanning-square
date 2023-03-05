#include "SAT2.h"
#include <list>
#include <stack>
#include <iostream>

typedef struct Node {
    int val;
    Node *next = nullptr;
    Node *prev = nullptr;
} Node;

typedef struct List {
    Node *head = nullptr;
    Node *tail = nullptr;
} List;

typedef struct {
    List *adj;
    int size;
} Graph;
Graph graph, invGraph;
int *SCC;

int index(int i) {
    if (i < 0)
        return -i * 2 - 1;
    return i * 2 - 2;
}

void insert(List *list, int val) {
    Node *node = new Node{};
    node->val = val;
    if (list->head == nullptr) {
        list->head = node;
        list->tail = node;
        return;
    }
    list->tail->next = node;
    node->prev = list->tail;
    list->tail = node;
}

void freeList(List *list) {
    Node *node = list->head;
    while (node != nullptr) {
        Node *next = node->next;
        delete node;
        node = next;
    }
}

void createGraphs(int *a, int *b, int n, int m) {
    graph = Graph{new List[2 * n], 2 * n};
    invGraph = Graph{new List[2 * n], 2 * n};
    for (int i = 0; i < m; i++) {
        insert(&graph.adj[index(-a[i])], index(b[i]));
        insert(&graph.adj[index(-b[i])], index(a[i]));
        insert(&invGraph.adj[index(b[i])], index(-a[i]));
        insert(&invGraph.adj[index(a[i])], index(-b[i]));
    }
}

int CC;

void dfs(Graph G, int node, bool *visited, stack<int> &stack) {
    SCC[node] = CC;
    visited[node] = true;
    for (auto it = G.adj[node].head; it != nullptr; it = it->next) {
        if (!visited[it->val])
            dfs(G, it->val, visited, stack);
    }
    stack.push(node);
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
    stack<int> s2;
    for (int i = 0; i < n; ++i) {
        visited[i] = false;
        SCC[i] = -1;
    }
    CC = 0;
    while (!s1.empty()) {
        if (visited[s1.top()]) {
            s1.pop();
            continue;
        }
        dfs(invGraph, s1.top(), visited, s2);
        s1.pop();
        CC++;
    }
    delete[] visited;
}

void deallocate() {
    delete[] SCC;
    for (int i = 0; i < graph.size; ++i) {
        freeList(&graph.adj[i]);
        freeList(&invGraph.adj[i]);
    }
    delete[] graph.adj;
    delete[] invGraph.adj;

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