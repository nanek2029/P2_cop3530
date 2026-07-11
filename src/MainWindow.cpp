#include "MainWindow.h"
#include <QVBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // QMessageBox::information(nullptr, "Status", "MainWindow is being built!"); (for future debugging use)

    // Create container/layout for widgets
    auto *centralWidget = new QWidget(this);
    auto *layout = new QVBoxLayout(centralWidget);

    // Textbox for stats
    resultDisplay = new QTextEdit(this);
    resultDisplay->setReadOnly(true);

    // Editable Textbox for actor names
    actor1 = new QLineEdit(this);
    actor2 = new QLineEdit(this);

    auto *searchButton = new QPushButton("Connect!", this);

    layout->addWidget(actor1);
    layout->addWidget(actor2);
    layout->addWidget(searchButton);
    layout->addWidget(resultDisplay);

    setCentralWidget(centralWidget);

    loadAllData(data); // load BTS graph

    connect(searchButton, &QPushButton::clicked, this, &MainWindow::onConnectClicked);
}

void MainWindow::onConnectClicked() {
    std::string s1 = actor1->text().toStdString();
    std::string s2 = actor2->text().toStdString();

    // TODO: check if actors exist to prevent crashes

    // obtain actor IDs for graph lookup
    int startID = data.idToNum[data.nameToActorID[s1][0]];
    int targetID = data.idToNum[data.nameToActorID[s2][0]];

    SearchResult bfs = BFS(data.graph, startID, targetID);
    SearchResult bidirect = bidirectBFS(data.graph, startID, targetID);


    // display findings
    QString output = "Bidirectional BFS Results:\n";
    output += "Nodes visited: " + QString::number(bidirect.nodesVisited) + "\n";
    output += "Duration: " + QString::number(bidirect.duration) + "ms\n";

    output += "Standard BFS Results:\n";
    output += "Nodes visited: " + QString::number(bfs.nodesVisited) + "\n";
    output += "Duration: " + QString::number(bfs.duration) + "ms\n";

    resultDisplay->setText(output);

}