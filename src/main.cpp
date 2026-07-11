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
#include "MainWindow.h"
#include <QApplication>
#include <QMessageBox>

using namespace std;




int main(int argc, char *argv[]
    ) {
    QApplication app(argc, argv);
    QMessageBox::information(nullptr, "Status", "App is starting");
    MainWindow window;
    window.show();

    return app.exec();
}
