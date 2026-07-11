#ifndef P2_COP3530_MAINWINDOW_H
#define P2_COP3530_MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <string>
#include "logic.h"
#include <QComboBox>
#include <QGraphicsScene> // container
#include <QGraphicsView> // ui component
#include <QGraphicsEllipseItem> // for nodes
#include <QGraphicsLineItem> // for edges
#include <QWheelEvent>
#include <QHBoxLayout>

using namespace std;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    void renderGraph(const std::vector<int>& path);     // For updating graph nodes & connections
    void populateDropdown(QComboBox &actor1, QComboBox &actor2) const;    // For including some sample actors into input dropdowns

private slots: // for event functionality
    void onConnectClicked();    // For checkmark button click (find connections between actors)
    void wheelEvent(QWheelEvent *event);    // For zooming in/out of graph

private:
    GraphData data;

    // UI widgets
    QComboBox *actor1;
    QComboBox *actor2;

    QPushButton *connectButton;

    QLabel *BFSTitle;
    QLabel *BFSNodesText;
    QLabel *BFSNodesValue;
    QLabel *BFSTimeText;
    QLabel *BFSTimeValue;

    QLabel *BiBFSTitle;
    QLabel *BiBFSNodesText;
    QLabel *BiBFSNodesValue;
    QLabel *BiBFSTimeText;
    QLabel *BiBFSTimeValue;

    QLabel *degOfSepText;
    QLabel *degOfSepValue;

    QLabel *title;
    QLabel *ampersand;
    QFrame *topSeparatorBox;
    QFrame *middleSeparatorBox;
    QFrame *bottomRightSeparatorBox;

    QLabel *totalVerticesText;
    QLabel *totalEdgesText;
    QLabel *totalVerticesValue;
    QLabel *totalEdgesValue;


    QGraphicsScene *scene;
    QGraphicsView *view;

    // UI layouts

    QWidget *centralWidget;
    QHBoxLayout *mainLayout;

    QFrame *rightsideFrame;
    QVBoxLayout *rightsideLayout;

    QFrame *graphFrame;
    QVBoxLayout *graphLayout;

    QFrame *sidebarFrame;
    QVBoxLayout *sidebarLayout;

    QHBoxLayout *verticesLayout;
    QHBoxLayout *edgesLayout;
    QHBoxLayout *BFSNodesLayout;
    QHBoxLayout *BFSTimeLayout;
    QHBoxLayout *BiBFSNodesLayout;
    QHBoxLayout *BiBFSTimeLayout;




    // Assembly layouts

    QHBoxLayout *inputLayout;
    QHBoxLayout *degOfSepLayout;
};


#endif //P2_COP3530_MAINWINDOW_H