#include "DecisionNode.h"

DecisionNode::DecisionNode(const QString &text, QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent), m_textItem(new QGraphicsTextItem(this)), m_descriptionItem(new QGraphicsTextItem(this)), m_description(text) {
    setRect(-20, -20, 40, 40);
    setBrush(QBrush(Qt::lightGray));
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    setText(text);
    setDescription("");
}

void DecisionNode::setText(const QString &text) {
    m_textItem->setPlainText(text);
    m_textItem->setPos(-m_textItem->boundingRect().width() / 2, -m_textItem->boundingRect().height() / 2 - 15);
}

QString DecisionNode::text() const {
    return m_textItem->toPlainText();
}

void DecisionNode::setDescription(const QString &description) {
    m_description = description;
    m_descriptionItem->setPlainText(description);
    m_descriptionItem->setPos(-m_descriptionItem->boundingRect().width() / 2, -m_descriptionItem->boundingRect().height() / 2 + 15);
}

QString DecisionNode::description() const {
    return m_description;
}

void DecisionNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    bool ok;
    QString text = QInputDialog::getText(nullptr, "Edit Decision Node", "Decision Node text:", QLineEdit::Normal, this->text(), &ok);
    if (ok && !text.isEmpty()) {
        setText(text);
    }

    QString description = QInputDialog::getText(nullptr, "Edit Decision Node Description", "Decision Node description:", QLineEdit::Normal, this->description(), &ok);
    if (ok) {
        setDescription(description);
    }
    
    QGraphicsEllipseItem::mouseDoubleClickEvent(event);
}

