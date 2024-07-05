#ifndef PRIMARYNODE_H
#define PRIMARYNODE_H

#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QInputDialog>
#include <QList>
#include "DecisionNode.h"

class DecisionNode;
class Edge;

class PrimaryNode : public QGraphicsEllipseItem {
public:
    PrimaryNode(const QString &text, QGraphicsItem *parent = nullptr);

    void setText(const QString &text);
    QString text() const;

    void setDescription(const QString &description);
    QString description() const;

    void addDecisionNode(DecisionNode* decisionNode);
    void removeDecisionNode(DecisionNode* decisionNode);
    QList<DecisionNode*> getDecisionNodes() const;
    void updateEdges();

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    QGraphicsTextItem *m_textItem;
    QGraphicsTextItem *m_descriptionItem;
    QList<DecisionNode*> m_decisionNodes;
};

#endif // PRIMARYNODE_H

