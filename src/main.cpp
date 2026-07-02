#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <string_view>

using namespace std;

// helper function to convert string id to number id 
int getId(const string &s, unordered_map<string, int> &idToNum, vector<string> &numToId, vector<vector<int>> &graph) {

    auto it = idToNum.find(s);

    if (it != idToNum.end()) {
        return it->second;
    }

    int newId = idToNum.size();
    idToNum[s] = newId;
    numToId.push_back(s);
    graph.emplace_back();
    return newId;
}

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

int main() {

    // there are three main tsv files
    // a file with actors ids and their names for the name lookup
    // same for the movies
    // and the third has the actor ids and their movie ids

    const int maxNodes = 120000;

    unordered_map<string, int> idToNum;
    vector<string> numToId;
    idToNum.reserve(maxNodes * 2);

    unordered_map<string, string> actorNames; // id, name
    unordered_map<string, string> movieTitles; // id, title
    vector<vector<int>> graph;

    // loading data
    ifstream names("name.basics.tsv");
    ifstream movies("title.basics.tsv");
    ifstream relations("title.principals.tsv");

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
            movieTitles[string(id)] = string(title);
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
            actorNames[string(id)] = string(name);
        }
    }
    names.close();

    // build graph, actor - movie connections 
    getline(relations, line);
    while (getline(relations, line)){

        if ((int)idToNum.size() >= maxNodes) {
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
        if (movieTitles.find(mId) == movieTitles.end()) continue;

        string aId(actorId);
        int movieNumId = getId(mId, idToNum, numToId, graph);
        int actorNumId = getId(aId, idToNum, numToId, graph);

        // dont make the graph too big 
        if (movieNumId >= maxNodes || actorNumId >= maxNodes) continue;

        graph[movieNumId].push_back(actorNumId);
        graph[actorNumId].push_back(movieNumId);
    }
    relations.close();

    cout << "Actors: " << actorNames.size() << endl;
    cout << "Movies: " << movieTitles.size() << endl;
    cout << "Nodes in graph: " << graph.size() << endl;

    for (int i = 0; i < 5; ++i) {

        // checkign if actor or movie id 
        if (actorNames.find(numToId[i]) != actorNames.end()) {

            cout << actorNames[numToId[i]] << " has " << graph[i].size() << " edges." << endl;

            // printing edges, should all be movies 
            for (int j = 0; j < graph[i].size(); ++j) {

                int neighborId = graph[i][j];
                string neighborStringId = numToId[neighborId];

                if (movieTitles.find(neighborStringId) != movieTitles.end()) {

                    string title = movieTitles[neighborStringId];
                    if (!title.empty()) {
                        cout << " -> " << title << endl;
                    }
                }
            }
        } else {
            // otherwise its a movie id 
            cout << movieTitles[numToId[i]] << " has " << graph[i].size() << " edges." << endl;

            for (int j = 0; j < graph[i].size(); ++j) {

                int neighborId = graph[i][j];
                string neighborStringId = numToId[neighborId];

                if (actorNames.find(neighborStringId) != actorNames.end()) {

                    string name = actorNames[neighborStringId];
                    if (!name.empty()) {
                        cout << " -> " << name << endl;
                    }
                }
            }
        }
    }

    return 0;
}
