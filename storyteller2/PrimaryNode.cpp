#include "PrimaryNode.h"

PrimaryNode::PrimaryNode(const QString &text, QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent), m_textItem(new QGraphicsTextItem(this)), m_descriptionItem(new QGraphicsTextItem(this)), m_decisionNodes() {
    setRect(-30, -30, 60, 60);
    setBrush(QBrush(Qt::lightGray));
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    setText(text);
    setDescription("");
}

void PrimaryNode::setText(const QString &text) {
    m_textItem->setPlainText(text);
    m_textItem->setPos(-m_textItem->boundingRect().width() / 2, -m_textItem->boundingRect().height() / 2 - 15);
}

QString PrimaryNode::text() const {
    return m_textItem->toPlainText();
}

void PrimaryNode::setDescription(const QString &description) {
    m_descriptionItem->setPlainText(description);
    m_descriptionItem->setPos(-m_descriptionItem->boundingRect().width() / 2, -m_descriptionItem->boundingRect().height() / 2 + 15);
}

QString PrimaryNode::description() const {
    return m_descriptionItem->toPlainText();
}

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
    for (DecisionNode* decisionNode : m_decisionNodes) {
        decisionNode->setPos(mapToScene(decisionNode->pos()));
    }
}

void PrimaryNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    bool ok;
    QString text = QInputDialog::getText(nullptr, "Edit Node", "Node text:", QLineEdit::Normal, this->text(), &ok);
    if (ok && !text.isEmpty()) {
        setText(text);
    }

    QString description = QInputDialog::getText(nullptr, "Edit Node Description", "Node description:", QLineEdit::Normal, this->description(), &ok);
    if (ok) {
        setDescription(description);
    }

    for (DecisionNode* decisionNode : m_decisionNodes) {
        QString decisionText = QInputDialog::getText(nullptr, "Edit Decision Node", "Decision Node text:", QLineEdit::Normal, decisionNode->text(), &ok);
        if (ok && !decisionText.isEmpty()) {
            decisionNode->setText(decisionText);
        }

        QString decisionDescription = QInputDialog::getText(nullptr, "Edit Decision Node Description", "Decision Node description:", QLineEdit::Normal, decisionNode->description(), &ok);
        if (ok) {
            decisionNode->setDescription(decisionDescription);
        }
    }

    QGraphicsEllipseItem::mouseDoubleClickEvent(event);
}

QVariant PrimaryNode::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionChange) {
        updateEdges();
    }
    return QGraphicsEllipseItem::itemChange(change, value);
}

