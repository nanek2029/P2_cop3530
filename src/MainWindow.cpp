#include "MainWindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QIcon>
#include <QFrame>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // QMessageBox::information(nullptr, "Status", "MainWindow is being built!"); (for future debugging use)

    // -*-*- WIDGET CREATION -*-*-
    // Editable Textbox for actor names
    actor1 = new QLineEdit(this);
    actor2 = new QLineEdit(this);
    actor1->setStyleSheet("border-radius: 10px; background-color: #e9d190; "
                          "color: #2a2929; font-size: 14px;"
                          "padding-left: 10px; padding-right: 10px;");
    actor1->setFixedHeight(40);
    actor1->setPlaceholderText("Actor 1");
    actor2->setStyleSheet("border-radius: 10px; background-color: #e9d190; "
                          "color: #2a2929; font-size: 14px;"
                          "padding-left: 10px; padding-right: 10px;");
    actor2->setFixedHeight(40);
    actor2->setPlaceholderText("Actor 2");

    auto *connectButton = new QPushButton("Connect!", this);
    connectButton->setFixedHeight(30);

    // Textbox for stats
    resultDisplay = new QTextEdit(this);
    resultDisplay->setReadOnly(true);

    auto *title = new QLabel("CINEVERSE");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: #2a2929; font-size: 38px;");
    auto *ampersand = new QLabel("&");
    ampersand->setAlignment(Qt::AlignCenter);

    auto *totalVerticesText = new QLabel("TOTAL VERTICES: ");
    auto *totalEdgesText = new QLabel("TOTAL EDGES: ");

    auto *topSeparatorBox = new QFrame;
    topSeparatorBox->setStyleSheet("background-color: #c79e73; border: none;");
    topSeparatorBox->setFixedHeight(4);

    auto *middleSeparatorBox = new QFrame;
    middleSeparatorBox->setStyleSheet("background-color: #c79e73; border: none;");
    middleSeparatorBox->setFixedHeight(4);



    // -*-*- LAYOUT/STRUCTURAL SETUP -*-*-
    // Main container
    auto *centralWidget = new QWidget(this);
    auto *mainLayout = new QHBoxLayout(centralWidget);\

    // Rightside container
    auto *rightsideFrame = new QFrame();
    auto *rightsideLayout = new QVBoxLayout(rightsideFrame);
    auto *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(actor1);
    inputLayout->addWidget(ampersand);
    inputLayout->addWidget(actor2);
    inputLayout->addWidget(connectButton);
    rightsideLayout->addStretch();
    rightsideLayout->addLayout(inputLayout);

    rightsideFrame->setStyleSheet("background-color: #2a2929; border: none;");
    inputLayout->setContentsMargins(30, 10, 30, 10);


    // Sidebar container
    auto *sidebarFrame = new QFrame();
    sidebarFrame->setFixedWidth(250);
    auto *sidebarLayout = new QVBoxLayout(sidebarFrame);

    sidebarLayout->addWidget(title);
    sidebarLayout->addWidget(topSeparatorBox);
    sidebarLayout->addWidget(totalVerticesText);
    sidebarLayout->addWidget(totalEdgesText);
    sidebarLayout->addWidget(middleSeparatorBox);
    sidebarLayout->addStretch();
    sidebarLayout->addWidget(resultDisplay);

    sidebarFrame->setStyleSheet("background-color: #e9d190; border: none;");
    sidebarFrame->setFrameStyle(QFrame::NoFrame);



    // Adding to main layout
    mainLayout->addWidget(sidebarFrame);
    mainLayout->addWidget(rightsideFrame, 1);

    setCentralWidget(centralWidget);

    loadAllData(data); // load BTS graph

    // -*-*- EVENT HANDLING -*-*-
    connect(connectButton, &QPushButton::clicked, this, &MainWindow::onConnectClicked);

    // Additional (basic) window stats
    this->setFixedSize(1300, 750);
    this->setWindowFlags(Qt::MSWindowsFixedSizeDialogHint); // greys out resize button
    this->setWindowTitle("CINEVERSE");
    // this->setWindowIcon(QIcon("relative filepath to be added when I find a nice icon"));
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