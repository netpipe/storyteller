#ifndef DECISIONNODE_H
#define DECISIONNODE_H

#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QInputDialog>

class DecisionNode : public QGraphicsEllipseItem {
public:
    DecisionNode(const QString &text, QGraphicsItem *parent = nullptr);

    void setText(const QString &text);
    QString text() const;

    void setDescription(const QString &description);
    QString description() const;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QGraphicsTextItem *m_textItem;
    QGraphicsTextItem *m_descriptionItem;
    QString m_description;
};

#endif // DECISIONNODE_H

