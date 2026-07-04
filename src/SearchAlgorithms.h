#ifndef P2_COP3530_SEARCHALGORITHMS_H
#define P2_COP3530_SEARCHALGORITHMS_H

#include <string>
#include <vector>
#include <unordered_map>

struct SearchResult {
    std::vector<std::string> path;
    int nodesVisited;
    double duration;

    SearchResult() {
        nodesVisited = 0;
        duration = 0.0;
    }
};

SearchResult BFS(
    const std::unordered_map<std::string, std::vector<std::string>>& graph,
    const std::string& start,
    const std::string& target);

SearchResult bidirectBFS(
    const std::unordered_map<std::string, std::vector<std::string>>& graph,
    const std::string& start,
    const std::string& target);

#endif //P2_COP3530_SEARCHALGORITHMS_H
