#include "SearchAlgorithms.h"
#include <algorithm>
#include <unordered_set>
#include <chrono>

SearchResult BFS(
    const std::unordered_map<std::string, std::vector<std::string>>& graph,
    const std::string& start,
    const std::string& target) {

    SearchResult result;
    auto timeStart = std::chrono::high_resolution_clock::now();
    if (graph.find(start) == graph.end() || graph.find(target) == graph.end()) {  // return an empty result is either actor/movie does not exist
        auto timeEnd = std::chrono::high_resolution_clock::now();
        result.duration = std::chrono::duration<double, std::milli>(timeEnd - timeStart).count();
        return result;
    }

    std::vector<std::string> q;  //vector acts as queue for BFS
    int indexFront = 0;
    std::unordered_set<std::string> visited;  // keeps track of nodes that were already visited
    std::unordered_map<std::string, std::string> parent;  // stores each node's parent so we can reconstruct shortest path
    q.push_back(start);
    visited.insert(start);

    while (indexFront < q.size()) {   // continue searching until there are no nodes left to search
        std::string curr = q[indexFront];
        indexFront++;

        result.nodesVisited++;

        if (curr == target) {   // if destination reached, rebuild shortest path
            std::string node = target;

            while (node != start) {   // walk backwards through parent map
                result.path.push_back(node);
                node = parent[node];
            }
            result.path.push_back(start);
            std::reverse(result.path.begin(), result.path.end());  // reverse to make path be going forward
            auto timeEnd = std::chrono::high_resolution_clock::now();
            result.duration = std::chrono::duration<double, std::milli>(timeEnd - timeStart).count();
            return result;
        }

        for (const std::string& next : graph.at(curr)) {   // visit every neighboring node
            if (visited.find(next) == visited.end()) {
                visited.insert(next);
                parent[next] = curr;
                q.push_back(next);
            }
        }
    }
    auto timeEnd = std::chrono::high_resolution_clock::now();
    result.duration = std::chrono::duration<double, std::milli>(timeEnd - timeStart).count();
    return result;
}

SearchResult bidirectBFS(
    const std::unordered_map<std::string, std::vector<std::string> > &graph,
    const std::string &start,
    const std::string &target) {

    SearchResult result;
    auto timeStart = std::chrono::high_resolution_clock::now();

    if (graph.find(start) == graph.end() || graph.find(target) == graph.end()) {
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

    std::vector<std::string> qStart;
    std::vector<std::string> qTarget;

    int frontStart = 0;
    int frontTarget = 0;

    std::unordered_set<std::string> visitedStart;   //visited from the start
    std::unordered_set<std::string> visitedTarget;  //visited from the target
    std::unordered_map<std::string, std::string> parentStart;
    std::unordered_map<std::string, std::string> parentTarget;

    qStart.push_back(start);
    qTarget.push_back(target);
    visitedStart.insert(start);
    visitedTarget.insert(target);

    std::string nodeMeet = "";

    while (frontStart < qStart.size() && frontTarget < qTarget.size()) {  // continue until one search runs out of nodes
        std::string currStart = qStart[frontStart];   // expand one level from the starting actor
        frontStart++;
        result.nodesVisited++;

        for (const std::string& next : graph.at(currStart)) {
            if (visitedStart.find(next) == visitedStart.end()) {
                visitedStart.insert(next);
                parentStart[next] = currStart;
                qStart.push_back(next);

                if (visitedTarget.find(next) != visitedTarget.end()) {
                    nodeMeet = next;  // stop when both searches meet
                    break;
                }
            }
        }
        if (!nodeMeet.empty()) {
            break;
        }

        std::string currTarget = qTarget[frontTarget];  // Expand one level from target actor
        frontTarget++;
        result.nodesVisited++;

        for (const std::string& next : graph.at(currTarget)) {
            if (visitedTarget.find(next) == visitedTarget.end()) {
                visitedTarget.insert(next);
                parentTarget[next] = currTarget;
                qTarget.push_back(next);

                if (visitedStart.find(next) != visitedStart.end()) {
                    nodeMeet = next;
                    break;
                }
            }
        }
        if (!nodeMeet.empty()) {
            break;
        }
    }
    if (nodeMeet.empty()) {   // no connection exists between the two actors
        auto timeEnd = std::chrono::high_resolution_clock::now();
        result.duration = std::chrono::duration<double, std::milli>(timeEnd - timeStart).count();
        return result;
    }

    std::vector<std::string> pathLeft;
    std::string node = nodeMeet;

    while (node != start) {
        pathLeft.push_back(node);
        node = parentStart[node];
    }
    pathLeft.push_back(start);
    std::reverse(pathLeft.begin(), pathLeft.end());
    std::vector<std::string> pathRight;
    node = nodeMeet;

    while (node != target) {
        node = parentTarget[node];
        pathRight.push_back(node);
    }
    result.path = pathLeft;   // combine both paths

    for (const std::string& item : pathRight) {
        result.path.push_back(item);
    }

    auto timeEnd = std::chrono::high_resolution_clock::now();
    result.duration = std::chrono::duration<double, std::milli>(timeEnd - timeStart).count();
    return result;
}
