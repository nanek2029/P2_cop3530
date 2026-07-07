#include "SearchAlgorithms.h"
#include <algorithm>
#include <queue>
#include <chrono>

SearchResult BFS(
    const std::vector<std::vector<int>>& graph,
    int start,
    int target) {

    SearchResult result;
    auto timeStart = std::chrono::high_resolution_clock::now();
    // Make sure start and target nodes exist in graph
    if (start < 0 || target < 0 || start >= graph.size() || target >= graph.size()) {
        auto timeEnd = std::chrono::high_resolution_clock::now();
        result.duration = std::chrono::duration<double, std::milli>(timeEnd - timeStart).count();
        return result;
    }

    std::vector<bool> visited(graph.size(), false);
    std::vector<int> parent(graph.size(), -1);
    std::queue<int> q;  //vector acts as queue for BFS
    q.push(start);
    visited[start] = true;

    while (!q.empty()) {   // continue searching until there are no nodes left to search
        int curr = q.front();
        q.pop();
        result.nodesVisited++;

        if (curr == target) {   // if destination reached, rebuild shortest path
            int node = target;

            while (node != -1) {   // walk backwards through parent map
                result.path.push_back(node);
                node = parent[node];
            }
            std::reverse(result.path.begin(), result.path.end());  // reverse to make path start at original actor and end at target actor
            auto timeEnd = std::chrono::high_resolution_clock::now();
            result.duration = std::chrono::duration<double, std::milli>(timeEnd - timeStart).count();
            return result;
        }

        for (int next : graph[curr]) {   // visit every neighboring node
            if (!visited[next]) {
                visited[next] = true;
                parent[next] = curr;  // stores this node
                q.push(next);
            }
        }
    }
    auto timeEnd = std::chrono::high_resolution_clock::now();
    result.duration = std::chrono::duration<double, std::milli>(timeEnd - timeStart).count();
    return result;
}

SearchResult bidirectBFS(
    const std::vector<std::vector<int>> &graph,
    int start,
    int target) {

    SearchResult result;
    auto timeStart = std::chrono::high_resolution_clock::now();

    if (start < 0 || target < 0 || start >= graph.size() || target >= graph.size()) {
        auto timeEnd = std::chrono::high_resolution_clock::now();
        result.duration = std::chrono::duration<double, std::milli>(timeEnd - timeStart).count();
        return result;
    }

    if (start == target) {   // if start and path are same, path is just the node
        result.path.push_back(start);
        result.nodesVisited = 1;
        auto timeEnd = std::chrono::high_resolution_clock::now();
        result.duration = std::chrono::duration<double, std::milli>(timeEnd - timeStart).count();
        return result;
    }

    std::queue<int> qStart;
    std::queue<int> qTarget;

    std::vector<bool> visitedStart(graph.size(), false);   //visited from the start
    std::vector<bool> visitedTarget(graph.size(), false);  //visited from the target
    std::vector<int> parentStart(graph.size(), -1);
    std::vector<int> parentTarget(graph.size(), -1);

    qStart.push(start);
    qTarget.push(target);
    visitedStart[start] = true;
    visitedTarget[target] = true;

    int nodeMeet = -1;

    while (!qStart.empty() && !qTarget.empty()) {  // continue until one search runs out of nodes
        int currStart = qStart.front();
        qStart.pop();
        result.nodesVisited++;

        for (int next : graph[currStart]) {
            if (!visitedStart[next]) {
                visitedStart[next] = true;
                parentStart[next] = currStart;
                qStart.push(next);

                if (visitedTarget[next]) {
                    nodeMeet = next;  // stop when both searches meet
                    break;
                }
            }
        }
        if (nodeMeet != -1) {
            break;
        }

        int currTarget = qTarget.front();  // Expand one level from target actor
        qTarget.pop();
        result.nodesVisited++;

        for (int next : graph[currTarget]) {
            if (!visitedTarget[next]) {
                visitedTarget[next] = true;
                parentTarget[next] = currTarget;
                qTarget.push(next);

                if (visitedStart[next]) {
                    nodeMeet = next;
                    break;
                }
            }
        }
        if (nodeMeet != -1) {
            break;
        }
    }
    if (nodeMeet == -1) {   // no connection exists between the two actors
        auto timeEnd = std::chrono::high_resolution_clock::now();
        result.duration = std::chrono::duration<double, std::milli>(timeEnd - timeStart).count();
        return result;
    }

    std::vector<int> pathLeft;
    int node = nodeMeet;

    while (node != -1) {
        pathLeft.push_back(node);
        node = parentStart[node];
    }
    std::reverse(pathLeft.begin(), pathLeft.end());
    std::vector<int> pathRight;
    node = nodeMeet;

    while (parentTarget[node] != -1) {
        node = parentTarget[node];
        pathRight.push_back(node);
    }
    result.path = pathLeft;   // combine both paths

    for (int item : pathRight) {
        result.path.push_back(item);
    }

    auto timeEnd = std::chrono::high_resolution_clock::now();
    result.duration = std::chrono::duration<double, std::milli>(timeEnd - timeStart).count();
    return result;
}