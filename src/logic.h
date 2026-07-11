#ifndef P2_COP3530_LOGIC_H
#define P2_COP3530_LOGIC_H

#include <vector>
#include <string>
#include <unordered_map>
#include "SearchAlgorithms.h"


struct GraphData {
    std::unordered_map<std::string, int> idToNum;
    std::vector<std::string> numToId;
    std::unordered_map<std::string, std::string> actorNames;
    std::unordered_map<std::string, std::string> movieTitles;
    std::unordered_map<std::string, std::vector<std::string>> nameToActorID;
    std::vector<std::vector<int>> graph;

    int getNumVertices() const {
        return static_cast<int>(graph.size());
    }

    long long getNumEdges() const {
        long long count = 0;
        for (const auto& neighbors : graph) {
            count += neighbors.size(); // Adds up links in graph
        }
        return count / 2; // Div by 2 since each edge is counted twice
    }
};

void loadAllData(GraphData& data);
void runLoop(GraphData& data);


#endif //P2_COP3530_LOGIC_H