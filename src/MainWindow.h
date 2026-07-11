#ifndef P2_COP3530_MAINWINDOW_H
#define P2_COP3530_MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include "logic.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private slots: // for event functionality
    void onConnectClicked();

private:
    GraphData data;
    QLineEdit *actor1;
    QLineEdit *actor2;
    QTextEdit *resultDisplay;
};


#endif //P2_COP3530_MAINWINDOW_H