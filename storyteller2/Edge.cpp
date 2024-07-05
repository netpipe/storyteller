#include "Edge.h"

Edge::Edge(PrimaryNode *startNode, DecisionNode *endNode, const QString &label, QGraphicsItem *parent)
    : QGraphicsLineItem(parent), m_startNode(startNode), m_endNode(endNode) {
    setPen(QPen(Qt::black, 2));
    m_label = new QGraphicsTextItem(label, this);
    updatePosition();
}

Edge::~Edge() {}

void Edge::updatePosition() {
    QLineF line(mapFromItem(m_startNode, 0, 0), mapFromItem(m_endNode, 0, 0));
    setLine(line);
    m_label->setPos((line.p1() + line.p2()) / 2);
}

PrimaryNode *Edge::startNode() const { return m_startNode; }
DecisionNode *Edge::endNode() const { return m_endNode; }
QString Edge::labelText() const { return m_label->toPlainText(); }

