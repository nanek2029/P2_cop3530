#include "MainWindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QIcon>
#include <QCompleter>
#include <QFrame>
#include <QComboBox>
#include <QString>
#include <QPushButton>

void MainWindow::renderGraph(const std::vector<int>& path) {

    scene->clear();

    int x_offset = 0;
    int node_distance = 120;

    for (int i = 0; i < path.size(); i++) { // for each node found in path
        int nodeID = path[i]; // retrieve actor ID from path vector

        // Draw connecting line (simply one big horizontal line)
        if (i > 0) {
            QGraphicsLineItem *line = scene->addLine(x_offset - (node_distance - 25), 25, x_offset + 25, 25, QPen(QColor("#2a2929"), 2));
            line->setZValue(0); // bottom layer
        }
        // Draw circle for curr node
        QGraphicsEllipseItem *node = scene->addEllipse(x_offset, 0, 50, 50,
                                    Qt::NoPen, QBrush(QColor("#a74b17")));
        node->setZValue(1); // middle layer

        // Create text label
        std::string imdbID = data.numToId[nodeID];
        std::string name;
        if (data.actorNames.count(imdbID)) // if ID found as actor name
            name = data.actorNames.at(imdbID);
        else if (data.movieTitles.count(imdbID)) // if ID found as movie name
            name = data.movieTitles.at(imdbID);
        else
            name = "Unknown"; // if any issues

        QGraphicsTextItem *label = scene->addText(QString::fromStdString(name));
        QFont font("Arial", 4);

        if (data.movieTitles.count(imdbID))
            font.setItalic(true); // italicize if movie title
        if (data.actorNames.count(imdbID))
            font.setBold(true); // bold if actor name

        label->setFont(font);
        label->setDefaultTextColor(Qt::white);

        // Create boundary for text
        QRectF textRect = label->boundingRect();
        label->setPos(x_offset + (50 - textRect.width()) / 2, 25 - (textRect.height() / 2));
        label->setZValue(2);

        x_offset += node_distance;
    }
    scene->update();
    view->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio); // Ensures graph loads with all nodes visible
}

void MainWindow::populateDropdown(QComboBox &actor1, QComboBox &actor2) {
    QStringList actors;
    for (int i = 1; i <= 9; ++i) {
        std::string id = "nm000000" + std::to_string(i); // look at IDs nm0000001 - nm0000009
        if (data.actorNames.count(id))
            actors.append(QString::fromStdString(data.actorNames.at(id)));
    }
    actor1.addItems(actors);
    actor2.addItems(actors);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

    loadAllData(data); // load BTS graph


    // -*-*- WIDGET CREATION -*-*-

    // Editable Combo Textbox for actor names

    actor1 = new QComboBox(this);
    actor1->setEditable(true);
    actor1->setMinimumWidth(300);
    actor1->lineEdit()->setPlaceholderText("Actor 1");
    // Autocomplete settings with basic actor names
    actor1->setCompleter(new QCompleter(actor1->model(), actor1));
    actor1->completer()->setCompletionMode(QCompleter::PopupCompletion);
    actor1->completer()->setCaseSensitivity(Qt::CaseInsensitive);

    actor2 = new QComboBox(this);
    actor2->setEditable(true);
    actor2->setMinimumWidth(300);
    actor2->lineEdit()->setPlaceholderText("Actor 2");
    // Autocomplete settings with basic actor names
    actor2->setCompleter(new QCompleter(actor2->model(), actor1));
    actor2->completer()->setCompletionMode(QCompleter::PopupCompletion);
    actor2->completer()->setCaseSensitivity(Qt::CaseInsensitive);

    populateDropdown(*actor1, *actor2);
    actor1->setCurrentIndex(-1);
    actor2->setCurrentIndex(-1);

    const QString actorStyle = "QComboBox {"
                            " border: 1px solid;"
                            " background-color: #e9d190;"
                            " color: #a74b17;"
                            " padding-left: 10px; padding-right: 10px;"
                            " border-radius: 10px;"
                            " font-size: 20px;"
                            " margin: 0px;"
                            " font-style: italic"
                            "}"
                            "QComboBox::drop-down {"
                            "  subcontrol-origin: margin;"
                            "  subcontrol-position: top right;"
                            "  width: 30px;"
                            "  border-left-width: 0px;"
                            "  border-top-right-radius: 20px;"
                            "  border-bottom-right-radius: 20px;"
                            "}"
                            "QComboBox::down-arrow {"
                            "  border-left: 5px solid transparent;"
                            "  border-right: 5px solid transparent;"
                            "  border-top: 6px solid #2a2929;"
                            "  margin-top: 2px;"
                            "}"
                            "QComboBox QAbstractItemView {"
                            "  border: 1px solid;"
                            "  background-color: #dcb080;"
                            "  border-radius: 10px;"
                            "  color: #2a2929;"
                            "}";

    actor1->setStyleSheet(actorStyle);
    actor1->setFixedHeight(60);
    actor2->setStyleSheet(actorStyle);
    actor2->setFixedHeight(60);

    // Circular Button for finding connection

    auto *connectButton = new QPushButton(this);
    connectButton->setStyleSheet("QPushButton {"
                            " border: none;"
                            " background-color: #a74b17;"
                            " border-radius: 22px;"
                            " margin: 0px;"
                            " padding: 0px;"
                            "}"
                            "QPushButton::hover {"
                            "  background-color: #6b341c;"
                            "}"
    );
    connectButton->setIcon(QIcon("./assets/checkmark.png"));
    connectButton->setIconSize(QSize(24,24));
    connectButton->setFixedSize(44,44);

    // *Blank* Degrees of Separation text
    auto *degOfSepText = new QLabel("Degrees of Separation");
    degOfSepText->setAlignment(Qt::AlignLeft);
    degOfSepText->setStyleSheet("font-size: 35px; color: #fffbf6;");
    degOfSepValue = new QLabel("?", this);
    degOfSepValue->setAlignment(Qt::AlignRight);
    degOfSepValue->setStyleSheet("font-size: 42px; font-weight: bold; color: #be874c;");

    // CINEVERSE Title text
    auto *title = new QLabel("CINEVERSE");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: #2a2929; font-size: 50px; font-weight: bold;");

    // "&" between actor inputs
    auto *ampersand = new QLabel("&");
    ampersand->setStyleSheet("font-size: 30px; font-weight: bold; color: #be874c;");
    ampersand->setAlignment(Qt::AlignCenter);

    // Total Vertices & Edges text/labels
    auto *totalVerticesText = new QLabel("TOTAL VERTICES: ");
    totalVerticesText->setStyleSheet("color: #2a2929; font-size: 18px; padding-left: 5px;");
    auto *totalEdgesText = new QLabel("TOTAL EDGES: ");
    totalEdgesText->setStyleSheet("color: #2a2929; font-size: 18px; padding-left: 5px;");

    auto *totalVerticesValue = new QLabel(QString::number(data.getNumVertices()));
    totalVerticesValue->setStyleSheet("color: #a74b17; font-size: 18px; padding-right: 5px; font-weight: bold;");
    totalVerticesValue->setAlignment(Qt::AlignRight);
    auto *totalEdgesValue = new QLabel(QString::number(data.getNumEdges()));
    totalEdgesValue->setStyleSheet("color: #a74b17; font-size: 18px; padding-right: 5px; font-weight: bold;");
    totalEdgesValue->setAlignment(Qt::AlignRight);

    // BFS Stats text/labels
    auto *BFSTitle = new QLabel("BREADTH-FIRST SEARCH");
    BFSTitle->setAlignment(Qt::AlignCenter);
    BFSTitle->setStyleSheet("color: #2a2929; font-size: 20px; font-weight: bold;");

    auto *BFSNodesText = new QLabel("NODES VISITED:");
    BFSNodesText->setStyleSheet("color: #2a2929; font-size: 18px; padding-left: 5px;");
    BFSNodesValue = new QLabel("0", this);
    BFSNodesValue->setStyleSheet("color: #a74b17; font-size: 18px; padding-right: 5px; font-weight: bold;");
    BFSNodesValue->setAlignment(Qt::AlignRight);

    auto *BFSTimeText = new QLabel("TIME TAKEN:");
    BFSTimeText->setStyleSheet("color: #2a2929; font-size: 18px; padding-left: 5px;");
    BFSTimeValue = new QLabel("0", this);
    BFSTimeValue->setStyleSheet("color: #a74b17; font-size: 18px; padding-right: 5px; font-weight: bold;");
    BFSTimeValue->setAlignment(Qt::AlignRight);

    // Bidirectional BFS Stats text/labels
    auto *BiBFSTitle = new QLabel("BIDIRECTIONAL BFS");
    BiBFSTitle->setAlignment(Qt::AlignCenter);
    BiBFSTitle->setStyleSheet("color: #2a2929; font-size: 20px; font-weight: bold;");

    auto *BiBFSNodesText = new QLabel("NODES VISITED:");
    BiBFSNodesText->setStyleSheet("color: #2a2929; font-size: 18px; padding-left: 5px;");
    BiBFSNodesValue = new QLabel("0", this);
    BiBFSNodesValue->setStyleSheet("color: #a74b17; font-size: 18px; padding-right: 5px; font-weight: bold;");
    BiBFSNodesValue->setAlignment(Qt::AlignRight);

    auto *BiBFSTimeText = new QLabel("TIME TAKEN:");
    BiBFSTimeText->setStyleSheet("color: #2a2929; font-size: 18px; padding-left: 5px;");
    BiBFSTimeValue = new QLabel("0", this);
    BiBFSTimeValue->setStyleSheet("color: #a74b17; font-size: 18px; padding-right: 5px; font-weight: bold;");
    BiBFSTimeValue->setAlignment(Qt::AlignRight);

    // Extra lines for style
    auto *topSeparatorBox = new QFrame;
    topSeparatorBox->setStyleSheet("background-color: #c79e73; border: none; margin: 0 10px;");
    topSeparatorBox->setFixedHeight(4);

    auto *middleSeparatorBox = new QFrame;
    middleSeparatorBox->setStyleSheet("background-color: #c79e73; border: none; margin: 0 40px;");
    middleSeparatorBox->setFixedHeight(4);

    auto *bottomRightSeparatorBox = new QFrame;
    bottomRightSeparatorBox->setStyleSheet("background-color: #c79e73; border: none;");
    bottomRightSeparatorBox->setFixedHeight(7);




    // -*-*- LAYOUT/STRUCTURAL SETUP -*-*-
    // Main container
    auto *centralWidget = new QWidget(this);
    auto *mainLayout = new QHBoxLayout(centralWidget);

    // Rightside container (for graph and inputs)
    auto *rightsideFrame = new QFrame();
    auto *rightsideLayout = new QVBoxLayout(rightsideFrame);

    // Graph container
    auto *graphFrame = new QFrame();
    auto *graphLayout = new QVBoxLayout(graphFrame);
    graphFrame->setStyleSheet("background-color: #fffbf6; border: none; border-radius: 10px;");
    graphFrame->setMinimumHeight(550);
    graphLayout->setContentsMargins(0,0,0,0);
    graphLayout->setSpacing(0);

    // Scene & View for graph
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, graphFrame);
    view->setStyleSheet("background: transparent; border: none;");
    view->setFrameStyle(QFrame::NoFrame);
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    view->setRenderHint(QPainter::Antialiasing);
    view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    view->setResizeAnchor(QGraphicsView::AnchorUnderMouse);

    // turn off scroll bars for graph
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);



    // -*-*- ASSEMBLING LAYOUT -*-*-

    auto *inputLayout = new QHBoxLayout();
    auto *degOfSepLayout = new QHBoxLayout();

    // Inputs horizontal part
    inputLayout->setSpacing(0); // global spacing for horizontal input space
    inputLayout->addSpacing(20);
    inputLayout->addWidget(actor1);

    inputLayout->addWidget(ampersand);

    inputLayout->addWidget(actor2);
    inputLayout->addSpacing(40);
    inputLayout->addWidget(connectButton);
    inputLayout->addSpacing(20);

    // Degrees of Separation text horizontal part
    degOfSepLayout->addStretch();
    degOfSepLayout->addWidget(degOfSepValue);
    degOfSepLayout->addSpacing(5);
    degOfSepLayout->addWidget(degOfSepText);
    degOfSepLayout->addStretch();

    // Whole right side
    rightsideLayout->addWidget(graphFrame);
    rightsideLayout->addStretch();
    rightsideLayout->addLayout(inputLayout);
    rightsideLayout->addLayout(degOfSepLayout);
    rightsideLayout->addStretch();
    rightsideLayout->addWidget(bottomRightSeparatorBox);

    rightsideFrame->setStyleSheet("background-color: #2a2929; border: none;");
    inputLayout->setContentsMargins(30, 10, 30, 10);

    // Graph
    graphLayout->addWidget(view);

    // Sidebar container
    auto *sidebarFrame = new QFrame();
    sidebarFrame->setFixedWidth(300);
    auto *sidebarLayout = new QVBoxLayout(sidebarFrame);

    // Total Vertices layout
    auto *verticesLayout = new QHBoxLayout();
    verticesLayout->addWidget(totalVerticesText);
    verticesLayout->addWidget(totalVerticesValue);
    // Total Edges layout
    auto *edgesLayout = new QHBoxLayout();
    edgesLayout->addWidget(totalEdgesText);
    edgesLayout->addWidget(totalEdgesValue);
    // BFS Nodes layout
    auto *BFSNodesLayout = new QHBoxLayout();
    BFSNodesLayout->addWidget(BFSNodesText);
    BFSNodesLayout->addWidget(BFSNodesValue);
    // BFS Time layout
    auto *BFSTimeLayout = new QHBoxLayout();
    BFSTimeLayout->addWidget(BFSTimeText);
    BFSTimeLayout->addWidget(BFSTimeValue);
    // BiBFS Nodes layout
    auto *BiBFSNodesLayout = new QHBoxLayout();
    BiBFSNodesLayout->addWidget(BiBFSNodesText);
    BiBFSNodesLayout->addWidget(BiBFSNodesValue);
    // BiBFS Time layout
    auto *BiBFSTimeLayout = new QHBoxLayout();
    BiBFSTimeLayout->addWidget(BiBFSTimeText);
    BiBFSTimeLayout->addWidget(BiBFSTimeValue);

    // Sidebar assmebly
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

    sidebarLayout->setSpacing(20);

    sidebarFrame->setStyleSheet("background-color: #e9d190; border: none;");
    sidebarFrame->setFrameStyle(QFrame::NoFrame);



    // Adding left & right side to main layout
    mainLayout->addWidget(sidebarFrame);
    mainLayout->addWidget(rightsideFrame, 1);

    setCentralWidget(centralWidget);



    // -*-*- EVENT HANDLING -*-*-
    connect(connectButton, &QPushButton::clicked, this, &MainWindow::onConnectClicked);


    // -*-*- EXTRA WINDOW STATS -*-*-
    this->setFixedSize(1300, 750);
    this->setWindowFlags(Qt::MSWindowsFixedSizeDialogHint); // greys out resize button
    this->setWindowTitle("CINEVERSE");
    this->setWindowIcon(QIcon("./assets/icon.png"));
}

void MainWindow::onConnectClicked() {

    // Converts QString input text to C++ strings
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
    std::string id1 = it1->second[0];
    std::string id2 = it2->second[0];

    // Check if id exists but has no connections
    if (data.idToNum.find(id1) == data.idToNum.end() || data.idToNum.find(id2) == data.idToNum.end()) {
        QMessageBox::warning(this, "DATA ERROR", "Actor exists but has NO movie connections currently OR is not an actor.");
        return;
    }

    int startID = data.idToNum.at(id1);
    int targetID = data.idToNum.at(id2);

    SearchResult bfs = BFS(data.graph, startID, targetID);
    SearchResult bidirect = bidirectBFS(data.graph, startID, targetID);

    // Alter BFS/BiBFS output stats
    BFSNodesValue->setText(QString::number(bfs.nodesVisited));
    BFSTimeValue->setText(QString::number(bfs.duration) + " ms");
    BiBFSNodesValue->setText(QString::number(bidirect.nodesVisited));
    BiBFSTimeValue->setText(QString::number(bidirect.duration) + " ms");

    // Alter graph
    if (!bfs.path.empty()) {
        renderGraph(bfs.path);
        int degrees_separation = static_cast<int>(bfs.path.size()) - 1;
        degOfSepValue->setText(QString::number(degrees_separation));
    }
    else {
        scene->clear();
        QMessageBox::information(this, "DATA ERROR", "No connection found.");
    }


}

void MainWindow::wheelEvent(QWheelEvent *event) {
    if (event->angleDelta().y() > 0) // looks at change of y movement (aka scroll wheel)
        view->scale(1.1, 1.1);
    else
        view->scale(0.9,0.9);
}