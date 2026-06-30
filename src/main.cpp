#include "bridges/Bridges.h"
#include "bridges/DataSource.h"
#include "bridges/data_src/ActorMovieIMDB.h"
#include <iostream>
#include <vector>

using namespace bridges;

int main() {

    Bridges bridges(0, "Nane_K", "75321214120");

    bridges.setTitle("IMDb Test");

    DataSource ds(&bridges);

    vector<ActorMovieIMDB> data = ds.getActorMovieIMDBData(100);

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

    // seeing contents of adjacency list / are nodes connected correctly 
    for (auto record = adjacency_list.begin(); record != adjacency_list.end(); record++) { 
        std::cout << "Node: " << record->first << std::endl; 
        std::cout << "Connected to: "; 

        for (auto neighbor : record->second) { 
            std::cout << neighbor << ", "; 
        }
        std::cout << std::endl;
    }



    return 0;
}