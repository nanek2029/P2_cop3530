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
};

void loadAllData(GraphData& data);
void runLoop(GraphData& data);


#endif //P2_COP3530_LOGIC_H