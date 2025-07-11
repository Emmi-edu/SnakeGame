//
// Created by emmit on 13/6/2025.
//
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
using namespace std;

// BFS 路径查找函数
vector<int> bfsShortestPath(vector<vector<int>>& graph, int start, int end) {
    queue<int> q;  // BFS 队列
    unordered_map<int, int> parent;  // 记录每个节点的父节点（前驱）
    vector<bool> visited(graph.size(), false);  // 访问标记

    q.push(start);
    visited[start] = true;
    parent[start] = -1;  // 起点无父节点

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        // 到达终点时回溯路径
        if (current == end) {
            vector<int> path;
            int node = end;
            // 从终点回溯到起点
            while (node != start) {
                path.push_back(node);
                node = parent[node];
            }
            path.push_back(start);  // 添加起点
            reverse(path.begin(), path.end());  // 反转路径顺序
            return path;
        }

        // 遍历当前节点的所有邻居
        for (int neighbor : graph[current]) {
            if (!visited[neighbor]) {
                q.push(neighbor);
                visited[neighbor] = true;
                parent[neighbor] = current;  // 记录父节点
            }
        }
    }
    return {};  // 未找到路径返回空向量
}
