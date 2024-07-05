#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <QInputDialog>
#include <QVBoxLayout>
#include <QList>
#include <QPen>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsProxyWidget>
#include <QDebug>
#include <cmath>

// Forward declarations
class Edge;
class DecisionNode;

// Primary Node class definition
class PrimaryNode : public QGraphicsEllipseItem {
public:
    PrimaryNode(const QString &text, QGraphicsItem *parent = nullptr)
        : QGraphicsEllipseItem(parent), m_textItem(new QGraphicsTextItem(this)), m_decisionNodes() {
        setRect(-30, -30, 60, 60);
        setBrush(QBrush(Qt::lightGray));
        setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
        setText(text);
    }

    void setText(const QString &text) {
        m_textItem->setPlainText(text);
        m_textItem->setPos(-m_textItem->boundingRect().width() / 2, -m_textItem->boundingRect().height() / 2);
    }

    QString text() const {
        return m_textItem->toPlainText();
    }

    void addDecisionNode(DecisionNode* decisionNode);
    void removeDecisionNode(DecisionNode* decisionNode);
    QList<DecisionNode*> getDecisionNodes() const;
    void updateEdges();

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override {
        bool ok;
        QString text = QInputDialog::getText(nullptr, "Edit Node", "Node text:", QLineEdit::Normal, this->text(), &ok);
        if (ok && !text.isEmpty()) {
            setText(text);
        }
        QGraphicsEllipseItem::mouseDoubleClickEvent(event);
    }

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override {
        if (change == ItemPositionChange) {
            updateEdges();
        }
        return QGraphicsEllipseItem::itemChange(change, value);
    }

private:
    QGraphicsTextItem *m_textItem;
    QList<DecisionNode*> m_decisionNodes;
};

// Decision Node class definition
class DecisionNode : public QGraphicsEllipseItem {
public:
    DecisionNode(const QString &text, QGraphicsItem *parent = nullptr)
        : QGraphicsEllipseItem(parent), m_textItem(new QGraphicsTextItem(this)), m_description(text) {
        setRect(-20, -20, 40, 40);
        setBrush(QBrush(Qt::lightGray));
        setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
        setText(text);
    }

    void setText(const QString &text) {
        m_textItem->setPlainText(text);
        m_textItem->setPos(-m_textItem->boundingRect().width() / 2, -m_textItem->boundingRect().height() / 2);
    }

    QString text() const {
        return m_textItem->toPlainText();
    }

    void setDescription(const QString &description) {
        m_description = description;
    }

    QString description() const {
        return m_description;
    }

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override {
        bool ok;
        QString labelText = QInputDialog::getText(nullptr, "Edit Decision Node Label", "Decision Node Label:", QLineEdit::Normal, this->text(), &ok);
        if (ok && !labelText.isEmpty()) {
            setText(labelText);
        }

        QString descriptionText = QInputDialog::getText(nullptr, "Edit Decision Node Description", "Decision Node Description:", QLineEdit::Normal, this->description(), &ok);
        if (ok) {
            setDescription(descriptionText);
        }

        QGraphicsEllipseItem::mouseDoubleClickEvent(event);
    }

private:
    QGraphicsTextItem *m_textItem;
    QString m_description;
};

// Edge class definition
class Edge : public QGraphicsLineItem {
public:
    Edge(PrimaryNode *startNode, DecisionNode *endNode, const QString &label, QGraphicsItem *parent = nullptr)
        : QGraphicsLineItem(parent), m_startNode(startNode), m_endNode(endNode) {
        setPen(QPen(Qt::black, 2));
        m_label = new QGraphicsTextItem(label, this);
        updatePosition();
    }

    ~Edge() {}

    void updatePosition() {
        QLineF line(mapFromItem(m_startNode, 0, 0), mapFromItem(m_endNode, 0, 0));
        setLine(line);
        m_label->setPos((line.p1() + line.p2()) / 2);
    }

private:
    PrimaryNode *m_startNode;
    DecisionNode *m_endNode;
    QGraphicsTextItem *m_label;
};

// Define PrimaryNode methods
void PrimaryNode::addDecisionNode(DecisionNode* decisionNode) {
    m_decisionNodes.append(decisionNode);
}

void PrimaryNode::removeDecisionNode(DecisionNode* decisionNode) {
    m_decisionNodes.removeAll(decisionNode);
}

QList<DecisionNode*> PrimaryNode::getDecisionNodes() const {
    return m_decisionNodes;
}

void PrimaryNode::updateEdges() {
    for (DecisionNode* decisionNode : qAsConst(m_decisionNodes)) {
        decisionNode->setPos(mapToScene(decisionNode->pos()));
    }
}

// MainWindow class definition
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr)
        : QMainWindow(parent), view(new QGraphicsView(this)), scene(new QGraphicsScene(this)) {
        setCentralWidget(view);
        view->setScene(scene);

        QMenu *menu = menuBar()->addMenu(tr("Edit"));
        QAction *addPrimaryNodeAction = menu->addAction(tr("Add Primary Node"));
        QAction *addDecisionNodeAction = menu->addAction(tr("Add Decision Node"));
        QAction *addEdgeAction = menu->addAction(tr("Add Edge"));
        QAction *removeSelectedAction = menu->addAction(tr("Remove Selected"));

        connect(addPrimaryNodeAction, &QAction::triggered, this, &MainWindow::addPrimaryNode);
        connect(addDecisionNodeAction, &QAction::triggered, this, &MainWindow::addDecisionNode);
        connect(addEdgeAction, &QAction::triggered, this, &MainWindow::addEdge);
        connect(removeSelectedAction, &QAction::triggered, this, &MainWindow::removeSelectedItems);

        // Initialize the scene with some nodes
        initializeScene();
    }

    ~MainWindow() {}

private slots:
    void addPrimaryNode() {
        bool ok;
        QString text = QInputDialog::getText(this, tr("Add Primary Node"), tr("Primary Node text:"), QLineEdit::Normal, QString(), &ok);
        if (ok && !text.isEmpty()) {
            PrimaryNode *node = new PrimaryNode(text);
            scene->addItem(node);
            node->setPos(view->mapToScene(view->viewport()->rect().center()));
        }
    }

    void addDecisionNode() {
        bool ok;
        QString text = QInputDialog::getText(this, tr("Add Decision Node"), tr("Decision Node text:"), QLineEdit::Normal, QString(), &ok);
        if (ok && !text.isEmpty()) {
            QList<QGraphicsItem *> selectedItems = scene->selectedItems();
            if (!selectedItems.isEmpty()) {
                PrimaryNode *parentNode = qgraphicsitem_cast<PrimaryNode *>(selectedItems.first());
                if (parentNode) {
                    // Limit to three decision nodes per primary node
                    if (parentNode->getDecisionNodes().size() < 3) {
                        DecisionNode *node = new DecisionNode(text);
                        scene->addItem(node);
                        parentNode->addDecisionNode(node);
                        node->setPos(parentNode->pos() + QPointF(0, 50 + parentNode->getDecisionNodes().size() * 80));
                    }
                }
            }
        }
    }

    void addEdge() {
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

    void removeSelectedItems() {
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

private:
    QGraphicsView *view;
    QGraphicsScene *scene;

    void initializeScene() {
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
};

// main function
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}

#include "main.moc"
