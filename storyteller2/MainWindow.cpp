#include "MainWindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QInputDialog>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), view(new QGraphicsView(this)), scene(new QGraphicsScene(this)) {
    setCentralWidget(view);
    view->setScene(scene);
    initializeScene();

    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QAction *saveAction = fileMenu->addAction(tr("&Save Map"));
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveMap);

    QToolBar *toolbar = addToolBar(tr("Main Toolbar"));
    QAction *addPrimaryNodeAction = toolbar->addAction(tr("Add Primary Node"));
    connect(addPrimaryNodeAction, &QAction::triggered, this, &MainWindow::addPrimaryNode);

    QAction *addDecisionNodeAction = toolbar->addAction(tr("Add Decision Node"));
    connect(addDecisionNodeAction, &QAction::triggered, this, &MainWindow::addDecisionNode);

    QAction *addEdgeAction = toolbar->addAction(tr("Add Edge"));
    connect(addEdgeAction, &QAction::triggered, this, &MainWindow::addEdge);

    QAction *removeSelectedItemsAction = toolbar->addAction(tr("Remove Selected Items"));
    connect(removeSelectedItemsAction, &QAction::triggered, this, &MainWindow::removeSelectedItems);
}

MainWindow::~MainWindow() {}

void MainWindow::addPrimaryNode() {
    bool ok;
    QString text = QInputDialog::getText(this, tr("Add Primary Node"), tr("Node text:"), QLineEdit::Normal, QString(), &ok);
    if (ok && !text.isEmpty()) {
        PrimaryNode *node = new PrimaryNode(text);
        scene->addItem(node);
        node->setPos(view->mapToScene(view->viewport()->rect().center()));
    }
}

void MainWindow::addDecisionNode() {
    QList<QGraphicsItem *> selectedItems = scene->selectedItems();
    if (selectedItems.size() == 1) {
        PrimaryNode *parentNode = qgraphicsitem_cast<PrimaryNode *>(selectedItems.first());
        if (parentNode) {
            bool ok;
            QString text = QInputDialog::getText(this, tr("Add Decision Node"), tr("Node text:"), QLineEdit::Normal, QString(), &ok);
            if (ok && !text.isEmpty()) {
                DecisionNode *node = new DecisionNode(text);
                scene->addItem(node);
                parentNode->addDecisionNode(node);
                node->setPos(parentNode->pos() + QPointF(0, 50 + parentNode->getDecisionNodes().size() * 80));
            }
        }
    }
}

void MainWindow::addEdge() {
    bool ok;
    QString label = QInputDialog::getText(this, tr("Add Edge"), tr("Edge label:"), QLineEdit::Normal, QString(), &ok);
    if (ok && !label.isEmpty()) {
        QList<QGraphicsItem *> selectedItems = scene->selectedItems();
        if (selectedItems.size() == 2) {
            PrimaryNode *startNode = qgraphicsitem_cast<PrimaryNode *>(selectedItems.first());
            DecisionNode *endNode = qgraphicsitem_cast<DecisionNode *>(selectedItems.last());
            if (startNode && endNode && startNode->getDecisionNodes().contains(endNode)) {
                Edge *edge = new Edge(startNode, endNode, label);
                scene->addItem(edge);
                edge->updatePosition();
            }
        }
    }
}

void MainWindow::removeSelectedItems() {
    QList<QGraphicsItem *> selectedItems = scene->selectedItems();
    for (QGraphicsItem *item : selectedItems) {
        if (Edge *edge = qgraphicsitem_cast<Edge *>(item)) {
            delete edge;
        } else if (PrimaryNode *primaryNode = qgraphicsitem_cast<PrimaryNode *>(item)) {
            QList<DecisionNode *> decisionNodes = primaryNode->getDecisionNodes();
            for (DecisionNode *decisionNode : decisionNodes) {
                scene->removeItem(decisionNode);
                delete decisionNode;
            }
            delete primaryNode;
        } else if (DecisionNode *decisionNode = qgraphicsitem_cast<DecisionNode *>(item)) {
            scene->removeItem(decisionNode);
            delete decisionNode;
        }
    }
}

void MainWindow::saveMap() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Map"), "", tr("JSON Files (*.json)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly)) {
            QJsonObject jsonMap;

            // Save nodes
            QJsonArray nodesArray;
            QList<QGraphicsItem *> items = scene->items();
            for (QGraphicsItem *item : items) {
                if (PrimaryNode *primaryNode = qgraphicsitem_cast<PrimaryNode *>(item)) {
                    QJsonObject nodeObject;
                    nodeObject["type"] = "primary";
                    nodeObject["text"] = primaryNode->text();
                    nodeObject["description"] = primaryNode->description();
                    nodeObject["x"] = primaryNode->pos().x();
                    nodeObject["y"] = primaryNode->pos().y();
                    nodesArray.append(nodeObject);

                    // Save connected decision nodes
                    QList<DecisionNode *> decisionNodes = primaryNode->getDecisionNodes();
                    for (DecisionNode *decisionNode : decisionNodes) {
                        QJsonObject decisionObject;
                        decisionObject["type"] = "decision";
                        decisionObject["text"] = decisionNode->text();
                        decisionObject["description"] = decisionNode->description();
                        decisionObject["x"] = decisionNode->pos().x();
                        decisionObject["y"] = decisionNode->pos().y();
                        nodesArray.append(decisionObject);
                    }
                }
            }
            jsonMap["nodes"] = nodesArray;

            // Save edges
            QJsonArray edgesArray;
            for (QGraphicsItem *item : items) {
                if (Edge *edge = qgraphicsitem_cast<Edge *>(item)) {
                    QJsonObject edgeObject;
                    edgeObject["start_node_text"] = edge->startNode()->text();
                    edgeObject["end_node_text"] = edge->endNode()->text();
                    edgeObject["label"] = edge->labelText();
                    edgesArray.append(edgeObject);
                }
            }
            jsonMap["edges"] = edgesArray;

            QJsonDocument saveDoc(jsonMap);
            file.write(saveDoc.toJson());
            file.close();
        }
    }
}

void MainWindow::initializeScene() {
    // Example initialization with nodes and edges
    PrimaryNode *primaryNode1 = new PrimaryNode("Primary Node 1");
    PrimaryNode *primaryNode2 = new PrimaryNode("Primary Node 2");
    DecisionNode *decisionNode1 = new DecisionNode("Decision 1");
    DecisionNode *decisionNode2 = new DecisionNode("Decision 2");
    DecisionNode *decisionNode3 = new DecisionNode("Decision 3");

    scene->addItem(primaryNode1);
    scene->addItem(primaryNode2);
    scene->addItem(decisionNode1);
    scene->addItem(decisionNode2);
    scene->addItem(decisionNode3);

    primaryNode1->setPos(-150, 0);
    primaryNode2->setPos(150, 0);

    primaryNode1->addDecisionNode(decisionNode1);
    primaryNode1->addDecisionNode(decisionNode2);
    primaryNode1->addDecisionNode(decisionNode3);

    Edge *edge1 = new Edge(primaryNode1, decisionNode1, "Edge Label 1");
    Edge *edge2 = new Edge(primaryNode1, decisionNode2, "Edge Label 2");
    Edge *edge3 = new Edge(primaryNode1, decisionNode3, "Edge Label 3");

    scene->addItem(edge1);
    scene->addItem(edge2);
    scene->addItem(edge3);

    edge1->updatePosition();
    edge2->updatePosition();
    edge3->updatePosition();
}
