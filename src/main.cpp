#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <string_view>
#include "SearchAlgorithms.h"
#include "logic.h"



using namespace std;


int main(
    ) {
    GraphData data;
    loadAllData(data);
    runLoop(data);
    return 0;

}
