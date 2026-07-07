#ifndef P2_COP3530_SEARCHALGORITHMS_H
#define P2_COP3530_SEARCHALGORITHMS_H

#include <string>
#include <vector>
#include <unordered_map>

struct SearchResult {
    std::vector<int> path;
    int nodesVisited = 0;
    double duration = 0.0;
};

SearchResult BFS(
    const std::vector<std::vector<int>>& graph,
    int start,
    int target);

SearchResult bidirectBFS(
    const std::vector<std::vector<int>>& graph,
    int start,
    int target);

#endif //P2_COP3530_SEARCHALGORITHMS_H