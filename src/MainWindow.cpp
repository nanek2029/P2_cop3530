#include "MainWindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QIcon>
#include <QCompleter>
#include <QFrame>
#include <QComboBox>
#include <QString>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // QMessageBox::information(nullptr, "Status", "MainWindow is being built!"); (for future debugging use)

    loadAllData(data); // load BTS graph

    // -*-*- WIDGET CREATION -*-*-

    // Editable Combo Textbox for actor names
    actor1 = new QComboBox(this);
    actor1->setEditable(true);
    actor1->setMinimumWidth(300);
    actor1->addItem("Humphrey Bogart"); // testing dropdown functionality
    actor1->addItem("Humrrrhrey Bogart"); // testing dropdown functionality
    actor1->setCurrentIndex(-1);
    actor1->lineEdit()->setPlaceholderText("Actor 1");

    actor1->setCompleter(new QCompleter(actor1->model(), actor1));
    actor1->completer()->setCompletionMode(QCompleter::PopupCompletion);
    actor1->completer()->setCaseSensitivity(Qt::CaseInsensitive);

    actor2 = new QComboBox(this);
    actor2->setEditable(true);
    actor2->setMinimumWidth(300);
    actor2->addItem("Humphrey Bogart"); // testing dropdown functionality
    actor2->addItem("Humrrrhrey Bogart"); // testing dropdown functionality
    actor2->setCurrentIndex(-1);
    actor2->lineEdit()->setPlaceholderText("Actor 2");

    actor2->setCompleter(new QCompleter(actor2->model(), actor1));
    actor2->completer()->setCompletionMode(QCompleter::PopupCompletion);
    actor2->completer()->setCaseSensitivity(Qt::CaseInsensitive);

    const QString actorStyle = "QComboBox {"
                            " border: 1px solid;"
                            " background-color: #e9d190;"
                            " color: #2a2929;"
                            " padding-left: 10px; padding-right: 10px;"
                            " border-radius: 10px;"
                            " font-size: 16px;"
                            "}"
                            "QComboBox::drop-down {"
                            "  subcontrol-origin: margin;"
                            "  subcontrol-position: top right;"
                            "  width: 30px;"
                            "  border-left-width: 0px;"
                            "  border-top-right-radius: 20px;"
                            "  border-bottom-right-radius: 20px;"
                            "}"
                            "QComboBox::down-arrow {" // want to replace w/ icon
                            "  border-left: 5px solid transparent;"
                            "  border-right: 5px solid transparent;"
                            "  border-top: 6px solid #2a2929;"
                            "  margin-top: 2px;"
                            "}"
                            "QComboBox QAbstractItemView {"
                            " border: 1px solid;"
                            " background-color: #dcb080;"
                            " border-radius: 10px;"
                            "}";

    actor1->setStyleSheet(actorStyle);
    actor1->setFixedHeight(60);
    actor2->setStyleSheet(actorStyle);
    actor2->setFixedHeight(60);




    auto *connectButton = new QPushButton("Connect!", this);
    connectButton->setStyleSheet("QPushButton {"
                            " border: none;"
                            " background-color: #e9d190;"
                            " color: #2a2929;"
                            " border-radius: 10px;"
                            "}"
                            "QPushButton::hover {"
                            "  background-color: #dcb080;"
                            "}"
    );
    connectButton->setFixedHeight(30);

    // Textbox for stats
    resultDisplay = new QTextEdit(this);
    resultDisplay->setReadOnly(true);

    // *Blank* Degrees of Separation text
    auto *degOfSepText = new QLabel("Degrees of Separation");
    degOfSepText->setAlignment(Qt::AlignLeft);
    auto *degOfSepLabel = new QLabel("3");
    degOfSepLabel->setAlignment(Qt::AlignRight);

    // Title text
    auto *title = new QLabel("CINEVERSE");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: #2a2929; font-size: 50px; font-weight: bold;");
    auto *ampersand = new QLabel("&");
    ampersand->setStyleSheet("font-size: 36");
    ampersand->setAlignment(Qt::AlignCenter);

    // Total Vertices & Edges text/labels
    auto *totalVerticesText = new QLabel("TOTAL VERTICES: ");
    totalVerticesText->setStyleSheet("color: #2a2929; font-size: 18px; padding-left: 5px;");
    auto *totalEdgesText = new QLabel("TOTAL EDGES: ");
    totalEdgesText->setStyleSheet("color: #2a2929; font-size: 18px; padding-left: 5px;");

    auto *totalVerticesLabel = new QLabel(QString::number(data.getNumVertices()));
    totalVerticesLabel->setStyleSheet("color: #a74b17; font-size: 18px; padding-right: 5px;");
    totalVerticesLabel->setAlignment(Qt::AlignRight);
    auto *totalEdgesLabel = new QLabel(QString::number(data.getNumEdges()));
    totalEdgesLabel->setStyleSheet("color: #a74b17; font-size: 18px; padding-right: 5px;");
    totalEdgesLabel->setAlignment(Qt::AlignRight);

    // BFS Stats text/labels
    auto *BFSTitle = new QLabel("BREADTH-FIRST SEARCH");
    BFSTitle->setAlignment(Qt::AlignCenter);
    BFSTitle->setStyleSheet("color: #2a2929; font-size: 20px; font-weight: bold;");

    auto *BFSNodesText = new QLabel("NODES VISITED:");
    BFSNodesText->setStyleSheet("color: #2a2929; font-size: 18px; padding-left: 5px;");
    BFSNodesValue = new QLabel("0", this);
    BFSNodesValue->setStyleSheet("color: #a74b17; font-size: 18px; padding-right: 5px;");
    BFSNodesValue->setAlignment(Qt::AlignRight);

    auto *BFSTimeText = new QLabel("TIME TAKEN:");
    BFSTimeText->setStyleSheet("color: #2a2929; font-size: 18px; padding-left: 5px;");
    BFSTimeValue = new QLabel("0", this);
    BFSTimeValue->setStyleSheet("color: #a74b17; font-size: 18px; padding-right: 5px;");
    BFSTimeValue->setAlignment(Qt::AlignRight);

    // Bidirectional BFS Stats text/labels
    auto *BiBFSTitle = new QLabel("BIDIRECTIONAL BFS");
    BiBFSTitle->setAlignment(Qt::AlignCenter);
    BiBFSTitle->setStyleSheet("color: #2a2929; font-size: 20px; font-weight: bold;");

    auto *BiBFSNodesText = new QLabel("NODES VISITED:");
    BiBFSNodesText->setStyleSheet("color: #2a2929; font-size: 18px; padding-left: 5px;");
    BiBFSNodesValue = new QLabel("0", this);
    BiBFSNodesValue->setStyleSheet("color: #a74b17; font-size: 18px; padding-right: 5px;");
    BiBFSNodesValue->setAlignment(Qt::AlignRight);

    auto *BiBFSTimeText = new QLabel("TIME TAKEN:");
    BiBFSTimeText->setStyleSheet("color: #2a2929; font-size: 18px; padding-left: 5px;");
    BiBFSTimeValue = new QLabel("0", this);
    BiBFSTimeValue->setStyleSheet("color: #a74b17; font-size: 18px; padding-right: 5px;");
    BiBFSTimeValue->setAlignment(Qt::AlignRight);

    // Extra lines for style
    auto *topSeparatorBox = new QFrame;
    topSeparatorBox->setStyleSheet("background-color: #c79e73; border: none;");
    topSeparatorBox->setFixedHeight(4);

    auto *middleSeparatorBox = new QFrame;
    middleSeparatorBox->setStyleSheet("background-color: #c79e73; border: none;");
    middleSeparatorBox->setFixedHeight(4);

    auto *bottomRightSeparatorBox = new QFrame;
    bottomRightSeparatorBox->setStyleSheet("background-color: #c79e73; border: none;");
    bottomRightSeparatorBox->setFixedHeight(7);




    // -*-*- LAYOUT/STRUCTURAL SETUP -*-*-
    // Main container
    auto *centralWidget = new QWidget(this);
    auto *mainLayout = new QHBoxLayout(centralWidget);

    // Rightside container
    auto *rightsideFrame = new QFrame();
    auto *rightsideLayout = new QVBoxLayout(rightsideFrame);

    auto *graphFrame = new QFrame();
    auto *graphLayout = new QVBoxLayout(graphFrame);
    graphLayout->setContentsMargins(30,30,30,30);
    graphFrame->setStyleSheet("background-color: #FFFFFF; border: 20px solid #2a2929; border-radius: 10px;");
    graphFrame->setMinimumHeight(550);
    auto *inputLayout = new QHBoxLayout();
    auto *degOfSepLayout = new QHBoxLayout();

    inputLayout->addSpacing(40);
    inputLayout->addWidget(actor1);
    inputLayout->addSpacing(20);
    inputLayout->addWidget(ampersand);
    inputLayout->addSpacing(20);
    inputLayout->addWidget(actor2);
    inputLayout->addSpacing(40);
    inputLayout->addWidget(connectButton);
    inputLayout->addSpacing(40);

    degOfSepLayout->addWidget(degOfSepLabel);
    degOfSepLayout->addWidget(degOfSepText);

    rightsideLayout->addWidget(graphFrame);
    rightsideLayout->addStretch();
    rightsideLayout->addLayout(inputLayout);
    rightsideLayout->addLayout(degOfSepLayout);
    rightsideLayout->addStretch();
    rightsideLayout->addWidget(bottomRightSeparatorBox);

    rightsideFrame->setStyleSheet("background-color: #2a2929; border: none;");
    inputLayout->setContentsMargins(30, 10, 30, 10);


    // Sidebar container
    auto *sidebarFrame = new QFrame();
    sidebarFrame->setFixedWidth(300);
    auto *sidebarLayout = new QVBoxLayout(sidebarFrame);

    auto *verticesLayout = new QHBoxLayout();
    verticesLayout->addWidget(totalVerticesText);
    verticesLayout->addWidget(totalVerticesLabel);
    auto *edgesLayout = new QHBoxLayout();
    edgesLayout->addWidget(totalEdgesText);
    edgesLayout->addWidget(totalEdgesLabel);
    auto *BFSNodesLayout = new QHBoxLayout();
    BFSNodesLayout->addWidget(BFSNodesText);
    BFSNodesLayout->addWidget(BFSNodesValue);
    auto *BFSTimeLayout = new QHBoxLayout();
    BFSTimeLayout->addWidget(BFSTimeText);
    BFSTimeLayout->addWidget(BFSTimeValue);
    auto *BiBFSNodesLayout = new QHBoxLayout();
    BiBFSNodesLayout->addWidget(BiBFSNodesText);
    BiBFSNodesLayout->addWidget(BiBFSNodesValue);
    auto *BiBFSTimeLayout = new QHBoxLayout();
    BiBFSTimeLayout->addWidget(BiBFSTimeText);
    BiBFSTimeLayout->addWidget(BiBFSTimeValue);

    sidebarLayout->addWidget(title);
    sidebarLayout->addWidget(topSeparatorBox);
    sidebarLayout->addLayout(verticesLayout);
    sidebarLayout->addLayout(edgesLayout);
    sidebarLayout->addWidget(middleSeparatorBox);
    sidebarLayout->addWidget(BFSTitle);
    sidebarLayout->addLayout(BFSNodesLayout);
    sidebarLayout->addLayout(BFSTimeLayout);
    sidebarLayout->addWidget(BiBFSTitle);
    sidebarLayout->addLayout(BiBFSNodesLayout);
    sidebarLayout->addLayout(BiBFSTimeLayout);
    sidebarLayout->addStretch();
    sidebarLayout->addWidget(resultDisplay);

    sidebarLayout->setSpacing(20);

    sidebarFrame->setStyleSheet("background-color: #e9d190; border: none;");
    sidebarFrame->setFrameStyle(QFrame::NoFrame);



    // Adding to main layout
    mainLayout->addWidget(sidebarFrame);
    mainLayout->addWidget(rightsideFrame, 1);

    setCentralWidget(centralWidget);



    // -*-*- EVENT HANDLING -*-*-
    connect(connectButton, &QPushButton::clicked, this, &MainWindow::onConnectClicked);





    // Additional (basic) window stats
    this->setFixedSize(1300, 750);
    this->setWindowFlags(Qt::MSWindowsFixedSizeDialogHint); // greys out resize button
    this->setWindowTitle("CINEVERSE");
    // this->setWindowIcon(QIcon("relative filepath to be added when I find a nice icon"));
}

void MainWindow::onConnectClicked() {
    QString qs1 = actor1->currentText();
    std::string s1 = qs1.toStdString();
    QString qs2 = actor2->currentText();
    std::string s2 = qs2.toStdString();

    auto it1 = data.nameToActorID.find(s1);
    auto it2 = data.nameToActorID.find(s2);

    // Check if actors exist to prevent crashes
    if (it1 == data.nameToActorID.end() || it2 == data.nameToActorID.end()) {
        QMessageBox::warning(this, "INPUT ERROR", "One or both actors were not found in the database. Try again!");
        return;
    }

    // obtain actor IDs for graph lookup
    int startID = data.idToNum[it1->second[0]];
    int targetID = data.idToNum[it2->second[0]];

    SearchResult bfs = BFS(data.graph, startID, targetID);
    SearchResult bidirect = bidirectBFS(data.graph, startID, targetID);

    BFSNodesValue->setText(QString::number(bfs.nodesVisited));
    BFSTimeValue->setText(QString::number(bfs.duration) + " ms");
    BiBFSNodesValue->setText(QString::number(bidirect.nodesVisited));
    BiBFSTimeValue->setText(QString::number(bidirect.duration) + " ms");

    // display findings
    QString output = "Bidirectional BFS Results:\n";
    output += "Nodes visited: " + QString::number(bidirect.nodesVisited) + "\n";
    output += "Duration: " + QString::number(bidirect.duration) + "ms\n";

    output += "Standard BFS Results:\n";
    output += "Nodes visited: " + QString::number(bfs.nodesVisited) + "\n";
    output += "Duration: " + QString::number(bfs.duration) + "ms\n";

    resultDisplay->setText(output);

}