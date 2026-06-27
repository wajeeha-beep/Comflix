#pragma once

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class Graph {
private:
    int size;
    vector<vector<int>> adjList;

public:
    Graph(int n) {
        size = n;
        adjList.resize(n);
    }

    // ✔ SAFE EDGE ADD
    void addEdge(int u, int v) {

        if (u >= size || v >= size || u < 0 || v < 0)
            return;

        adjList[u].push_back(v);
        adjList[v].push_back(u); // undirected graph
    }

    // ✔ SAFE SIMPLE DISPLAY
    void showRecommendations(int movieId) {

        if (movieId < 0 || movieId >= size) return;

        cout << "Recommended Movies:\n";

        for (int id : adjList[movieId]) {
            cout << id << endl;
        }
    }

    // ✔ SAFE ACCESSOR
    vector<int> getRecommendations(int movieId) {

        if (movieId < 0 || movieId >= size)
            return {};

        return adjList[movieId];
    }

    vector<vector<int>> getAdjList() {
        return adjList;
    }

    void setAdjList(const vector<vector<int>>& data) {
        adjList = data;
    }

    // ✔ FIXED BFS (NO ERASE FRONT BUG)
    void bfs(int start) {

        if (start < 0 || start >= size) return;

        vector<bool> visited(size, false);
        queue<int> q;

        q.push(start);
        visited[start] = true;

        cout << "BFS Recommendations:\n";

        while (!q.empty()) {

            int node = q.front();
            q.pop();

            cout << "Movie ID: " << node << endl;

            for (int i : adjList[node]) {

                if (!visited[i]) {
                    visited[i] = true;
                    q.push(i);
                }
            }
        }
    }
};