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
    actor1->addItem("Humphrey Bogart"); // testing dropdown functionality
    actor1->addItem("Humrrrhrey Bogart"); // testing dropdown functionality
    actor1->setCurrentIndex(-1);
    actor1->lineEdit()->setPlaceholderText("Actor 1");

    actor1->setCompleter(new QCompleter(actor1->model(), actor1));
    actor1->completer()->setCompletionMode(QCompleter::PopupCompletion);
    actor1->completer()->setCaseSensitivity(Qt::CaseInsensitive);

    actor2 = new QComboBox(this);
    actor2->setEditable(true);
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
                            " background-color: #e9d190;"
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
    title->setStyleSheet("color: #2a2929; font-size: 38px; font-weight: bold;");
    auto *ampersand = new QLabel("&");
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

    // Extra lines for style
    auto *topSeparatorBox = new QFrame;
    topSeparatorBox->setStyleSheet("background-color: #c79e73; border: none;");
    topSeparatorBox->setFixedHeight(4);

    auto *middleSeparatorBox = new QFrame;
    middleSeparatorBox->setStyleSheet("background-color: #c79e73; border: none;");
    middleSeparatorBox->setFixedHeight(4);



    // -*-*- LAYOUT/STRUCTURAL SETUP -*-*-
    // Main container
    auto *centralWidget = new QWidget(this);
    auto *mainLayout = new QHBoxLayout(centralWidget);

    // Rightside container
    auto *rightsideFrame = new QFrame();
    auto *rightsideLayout = new QVBoxLayout(rightsideFrame);
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

    rightsideLayout->addStretch();
    rightsideLayout->addLayout(inputLayout);
    rightsideLayout->addLayout(degOfSepLayout);

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

    sidebarLayout->addWidget(title);
    sidebarLayout->addWidget(topSeparatorBox);
    sidebarLayout->addLayout(verticesLayout);
    sidebarLayout->addLayout(edgesLayout);
    sidebarLayout->addWidget(middleSeparatorBox);
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

    // TODO: check if actors exist to prevent crashes

    // obtain actor IDs for graph lookup
    int startID = data.idToNum[it1->second[0]];
    int targetID = data.idToNum[it2->second[0]];

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