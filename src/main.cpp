#include "bridges/Bridges.h"
#include "bridges/DataSource.h"
#include "bridges/data_src/ActorMovieIMDB.h"
#include "SearchAlgorithms.h"
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <vector>

using namespace bridges;

void printPath(const std::vector<std::string>& path) {
    if (path.empty()) {
        std::cout << "No connection found.\n";
        return;
    }

    if (path.size() == 1) {
        std::cout << path[0] << " is the same actor.\n";
        return;
    }

    for (int i = 0; i + 2 < path.size(); i += 2) {
        std::cout << path[i] << " acted in " << path[i + 1] << " with " << path[i + 2] << std::endl;
    }

    int degrees = (path.size() - 1) / 2;
    std::cout << "Degrees of separation: " << degrees << std::endl;
}

int main() {

    Bridges bridges(0, "Nane_K", "75321214120");

    bridges.setTitle("IMDb Test");

    DataSource ds(&bridges);

    vector<ActorMovieIMDB> data = ds.getActorMovieIMDBData(10000);

    // test, this is and all above is from the tutorial sample code in the website 
    std::cout << "First record:\n";
    std::cout << "Actor: " << data[0].getActor() << std::endl;
    std::cout << "Movie: " << data[0].getMovie() << std::endl;

    // below is me trying to parse the data 
    std::unordered_map<std::string, std::vector<std::string>> adjacency_list;
    std::unordered_set<std::string> actors;
    std::unordered_set<std::string> movies;

    for (ActorMovieIMDB record : data) {

        std::string actor = record.getActor();
        std::string movie = record.getMovie();

        if (actor.empty() || movie.empty()) continue;

        actors.insert(actor);
        movies.insert(movie);
        adjacency_list[actor].push_back(movie);
        adjacency_list[movie].push_back(actor);

    }

    // Test to check is BFS and Bidirectional BFS are working
    std::vector<std::pair<std::string, std::string>> pairsTest = {
        {"Kevin_Carroll", "Isaac_Hanson"},
        {"Billy_Crystal", "Kevin_Bacon_(I)"},
        {"Cary_Grant", "Clark_Gable"}
    };

    for (const auto& pair : pairsTest) {
        std::string actor1 = pair.first;
        std::string actor2 = pair.second;
        std::cout << "\n====================================\n";
        std::cout << "Testing: " <<actor1 << " -> " << actor2 << "\n";

        SearchResult bfsResult = BFS(adjacency_list, actor1, actor2);
        SearchResult bidirectResult = bidirectBFS(adjacency_list, actor1, actor2);

        std::cout << "\n========== BFS ==========\n";
        std::cout << "Start Actor: " << actor1 << std::endl;
        std::cout << "Target Actor: " << actor2 << std::endl;
        std::cout << "Nodes Visited: " << bfsResult.nodesVisited << std::endl;
        std::cout << "Time: " << bfsResult.duration << " ms\n";
        std::cout << "Shortest Path:\n";

        printPath(bfsResult.path);

        std::cout << "\n========== Bidirectional BFS ==========\n";
        std::cout << "Nodes Visited: " << bidirectResult.nodesVisited << std::endl;
        std::cout << "Time: " << bidirectResult.duration << " ms\n";
        std::cout << "Shortest Path:\n";

        printPath(bidirectResult.path);
    }

    // seeing contents of adjacency list / are nodes connected correctly
    // for (auto record = adjacency_list.begin(); record != adjacency_list.end(); record++) {
    //     std::cout << "Node: " << record->first << std::endl;
    //     std::cout << "Connected to: ";
    //
    //     for (auto neighbor : record->second) {
    //         std::cout << neighbor << ", ";
    //     }
    //     std::cout << std::endl;
    // }

    return 0;
}