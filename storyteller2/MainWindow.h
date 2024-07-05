#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QInputDialog>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "PrimaryNode.h"
#include "DecisionNode.h"
#include "Edge.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addPrimaryNode();
    void addDecisionNode();
    void addEdge();
    void removeSelectedItems();
    void saveMap();

private:
    QGraphicsView *view;
    QGraphicsScene *scene;

    void initializeScene();
};

#endif // MAINWINDOW_H

