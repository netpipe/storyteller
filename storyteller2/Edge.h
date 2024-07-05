#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include "PrimaryNode.h"
#include "DecisionNode.h"

class Edge : public QGraphicsLineItem {
public:
    Edge(PrimaryNode *startNode, DecisionNode *endNode, const QString &label, QGraphicsItem *parent = nullptr);
    ~Edge();

    void updatePosition();
    PrimaryNode *startNode() const;
    DecisionNode *endNode() const;
    QString labelText() const;

private:
    PrimaryNode *m_startNode;
    DecisionNode *m_endNode;
    QGraphicsTextItem *m_label;
};

#endif // EDGE_H

