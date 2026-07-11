#ifndef P2_COP3530_MAINWINDOW_H
#define P2_COP3530_MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>
#include <string>
#include "logic.h"
#include <QComboBox>
#include <QGraphicsScene> // container
#include <QGraphicsView> // ui component
#include <QGraphicsEllipseItem> // for nodes
#include <QGraphicsLineItem> // for edges
#include <QWheelEvent>

using namespace std;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    void renderGraph(const std::vector<int>& path);     // For updating graph nodes & connections
    void populateDropdown(QComboBox &actor1, QComboBox &actor2);    // For including some sample actors into input dropdowns

private slots: // for event functionality
    void onConnectClicked();    // For checkmark button click (find connections between actors)
    void wheelEvent(QWheelEvent *event);    // For zooming in/out of graph

private:
    GraphData data;
    QComboBox *actor1;
    QComboBox *actor2;

    QLabel *BFSNodesValue;
    QLabel *BFSTimeValue;
    QLabel *BiBFSNodesValue;
    QLabel *BiBFSTimeValue;

    QLabel *degOfSepValue;

    QGraphicsScene *scene;
    QGraphicsView *view;

    string iconFilepath = ""; // !!!! don't forget to include later
};


#endif //P2_COP3530_MAINWINDOW_H