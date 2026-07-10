#include "logic.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

// helper function to get the next field from a line of tsv data
static inline string_view getField(const string &line, int &pos) {

    int next = line.find('\t', pos);

    if (next == string::npos) {
        next = line.size();
    }

    string_view out(line.data() + pos, next - pos);
    pos = next + 1;

    if (out == "\\N") {
        return string_view();
    }

    return out;
}

// helper for printing path
void printPath(
    const SearchResult& result,
    const vector<string>& numToID,
    const unordered_map<string, string>& actorNames,
    const unordered_map<string, string>& movieTitles) {
    for (int node : result.path) {
        string imdbID = numToID[node];

        if (actorNames.find(imdbID) != actorNames.end()) {
            cout << actorNames.at(imdbID);
        } else if (movieTitles.find(imdbID) != movieTitles.end()) {
            cout << movieTitles.at(imdbID);
        }
        cout << endl;
    }
}


int getId(const std::string &s, GraphData &data) {
    auto it = data.idToNum.find(s);
    if (it != data.idToNum.end())
        return it->second;

    int newId = data.idToNum.size();
    data.idToNum[s] = newId;
    data.numToId.push_back(s);
    data.graph.emplace_back();
    return newId;
}

void loadAllData(GraphData& data) {
    // there are three main tsv files
    // a file with actors ids and their names for the name lookup
    // same for the movies
    // and the third has the actor ids and their movie ids
    const int maxNodes = 120000;
    data.idToNum.reserve(maxNodes * 2);

    ifstream names("src/name.basics.tsv");
    ifstream movies("src/title.basics.tsv");
    ifstream relations("src/title.principals.tsv");

    if (!names.is_open()) {
        cout << "names file did not open "<< endl;
    }
    if (!movies.is_open()) {
        cout << "movies file did not open "<< endl;
    }
    if (!relations.is_open()) {
        cout << "relations file did not open "<< endl;
    }


    // movie titles stored in map [id] = title
    string line;
    getline(movies, line);
    while (getline(movies, line)){

        int pos = 0;

        // the fields of the tsv line that are relevant are id, type, title
        string_view id = getField(line, pos);
        string_view type = getField(line, pos);
        string_view title = getField(line, pos);

        // only store movies, not tv shows or other types
        if (type == "movie" && !id.empty() && !title.empty()) {
            data.movieTitles[string(id)] = string(title);
        }
    }
    movies.close();

    // actor names stored in map [id] = name
    getline(names, line);
    while (getline(names, line)){

        int pos = 0;
        string_view id = getField(line, pos);
        string_view name = getField(line, pos);

        if (!id.empty() && !name.empty()) {
            //actorNames[string(id)] = string(name);
            string actorName(name);
            if (!actorName.empty() && actorName.back() == '\r') {
                actorName.pop_back();
            }
            data.actorNames[string(id)] = actorName;
            data.nameToActorID[actorName].push_back(string(id));
        }
    }
    names.close();

    // build graph, actor - movie connections
    getline(relations, line);
    while (getline(relations, line)){

        if ((int)data.idToNum.size() >= maxNodes) {
            break;
        }
        int pos = 0;
        string_view movieId = getField(line, pos);
        getField(line, pos);
        string_view actorId = getField(line, pos);
        string_view category = getField(line, pos);

        // only consider actors and actresses, skip directors, writers
        if (category != "actor" && category != "actress") continue;

        string mId(movieId);
        if (data.movieTitles.find(mId) == data.movieTitles.end()) continue;

        string aId(actorId);
        int movieNumId = getId(mId, data);
        int actorNumId = getId(aId, data);

        // dont make the graph too big
        if (movieNumId >= maxNodes || actorNumId >= maxNodes) continue;

        data.graph[movieNumId].push_back(actorNumId);
        data.graph[actorNumId].push_back(movieNumId);
    }
    relations.close();
}

void runLoop(GraphData&data) {
    while (true) {
        string startActor;
        cout << "\nEnter the starting actor's name: ";
        getline(cin, startActor);
        string startIMDb = data.nameToActorID[startActor][0];

        while (data.idToNum.find(startIMDb) == data.idToNum.end()) {
            cout << "\nActor not in graph. Try again: ";
            getline(cin, startActor);
            startIMDb = data.nameToActorID[startActor][0];
        }

        string targetActor;
        cout << "Enter the target actor's name: ";
        getline(cin, targetActor);
        string targetIMDb = data.nameToActorID[targetActor][0];

        while (data.idToNum.find(targetIMDb) == data.idToNum.end()) {
            cout << "\nActor not in graph. Try again: ";
            getline(cin, targetActor);
            targetIMDb = data.nameToActorID[targetActor][0];
        }

        int startID = data.idToNum[startIMDb];
        int targetID = data.idToNum[targetIMDb];

        //Run BFS logic and print results
        SearchResult bfsResult = BFS(data.graph, startID, targetID);
        SearchResult BidirectResult = bidirectBFS(data.graph, startID, targetID);

        cout << "\n========== BFS Result ==========" << endl;
        if (bfsResult.path.empty()) {
            cout << "\nNo connection found." << endl;
        } else {
            cout << "\nConnection found!" << endl;
            cout << "Nodes visited: " << bfsResult.nodesVisited << endl;
            cout << "Duration: " << bfsResult.duration << " ms" << endl;
            cout << "Shortest Path: " << bfsResult.path.size() << endl;
            printPath(bfsResult, data.numToId, data.actorNames, data.movieTitles);
        }

        cout << "\n===== Bidirectional BFS Result =====" << endl;
        if (BidirectResult.path.empty()) {
            cout << "\nNo connection found." << endl;
        } else {
            cout << "\nConnection found!" << endl;
            cout << "Nodes visited: " << BidirectResult.nodesVisited << endl;
            cout << "Duration: " << BidirectResult.duration << " ms" << endl;
            cout << "Shortest Path: " << BidirectResult.path.size() << endl;
            printPath(BidirectResult, data.numToId, data.actorNames, data.movieTitles);
        }

        cout << "\nEnter -1 to stop. Enter any other key to try another pair." << endl;
        string choice;
        getline(cin, choice);
        if (choice == "-1") {
            cout << "Exiting program." << endl;
            break;
        }
    }
}