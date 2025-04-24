#include "mindmapnode.hpp"
#include <QGraphicsSceneDragDropEvent>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QtMath>

MindmapNode::MindmapNode(size_t nodeId, const QString& content, const QBrush& brush, const QPen& pen, const QFont& font):
    _nodeId(nodeId),
    _brush(brush),
    _pen(pen),
    _font(font)
{
    auto sin = qRadiansToDegrees(qSin(qreal(_nodeId)));
    auto cos = qRadiansToDegrees(qCos(qreal(_nodeId)));

    setRect(cos * 4.0,
            sin * 4.0,
            120.0,
            40.0);

    setFlag(QGraphicsItem::ItemIsMovable);
    setBrush(_brush);

    setPen(_pen);

    _textContainer = new QGraphicsTextItem(this);
    _textContainer->setPos(sceneBoundingRect().x(), sceneBoundingRect().y());

    setContent(content);

    resize();
}

size_t MindmapNode::getNodeId() const
{
    return _nodeId;
}

void MindmapNode::setNodeId(const size_t &id)
{
    _nodeId = id;
}

QJsonValue MindmapNode::toJSON() const
{
    QJsonObject json;
    json["id"] = QVariant(static_cast<unsigned int>(_nodeId)).toInt();
    json["content"] = _content;
    json["description"] = _description;
    QJsonArray jsonArray;
    jsonArray = QJsonArray::fromStringList(_yes);
    json["yes"] = jsonArray;
    jsonArray = QJsonArray::fromStringList(_no);
    json["no"] = jsonArray;
    jsonArray = QJsonArray::fromStringList(_maybe);
    json["maybe"] = jsonArray;

    const auto& rect = sceneBoundingRect();

    json["x"] = rect.x();
    json["y"] = rect.y();
    json["w"] = rect.width();
    json["h"] = rect.height();

    return json;
}

void MindmapNode::fromJSON(const QJsonObject &jsonObject,int x1,int y1)
{
    auto x = jsonObject["x"].toDouble()+x1;
    auto y = jsonObject["y"].toDouble()+y1;
    auto w = jsonObject["w"].toDouble();
    auto h = jsonObject["h"].toDouble();
    setRect(x, y, w, h);
    _textContainer->setPos(sceneBoundingRect().x(), sceneBoundingRect().y());

    setNodeId(jsonObject["id"].toInt());
    setContent(jsonObject["content"].toString());
    setDescription(jsonObject["description"].toString());

    QJsonArray jsonArray;
    QStringList stringList;
    jsonArray = jsonObject["yes"].toArray();
    stringList.clear();
    for (const QJsonValue &value : jsonArray) stringList.append(value.toString());
    setYesList(stringList);
    jsonArray = jsonObject["no"].toArray();
    stringList.clear();
    for (const QJsonValue &value : jsonArray) stringList.append(value.toString());
    setNoList(stringList);
    jsonArray = jsonObject["maybe"].toArray();
    stringList.clear();
    for (const QJsonValue &value : jsonArray) stringList.append(value.toString());
    setMaybeList(stringList);
}

void MindmapNode::setContent(const QString& content)
{
    _content = content;
    _textContainer->setPlainText(_content);
}

QString MindmapNode::getContent() const
{
    return _content;
}

void MindmapNode::setYesList(const QStringList &yesList)
{
    _yes = yesList;
}

QStringList MindmapNode::getYesList() const
{
    return _yes;
}

void MindmapNode::setNoList(const QStringList &noList)
{
    _no = noList;
}

QStringList MindmapNode::getNoList() const
{
    return _no;
}

void MindmapNode::setMaybeList(const QStringList &maybeList)
{
    _maybe = maybeList;
}

QStringList MindmapNode::getMaybeList() const
{
    return _maybe;
}

void MindmapNode::setDescription(const QString &description)
{
    _description = description;
}

QString MindmapNode::getDescription() const
{
    return _description;
}

void MindmapNode::resize()
{
    auto _textRect = _textContainer->sceneBoundingRect();
    sceneBoundingRect().setRect(_textRect.x(), _textRect.y(), _textRect.width(), _textRect.height());
}

void MindmapNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // emit signal to the mindmap scene which will then re-calculate the corresponding edges
    emit positionChanged(this);

    // propagate event to the base object to prevent jumping of the rect
    QGraphicsRectItem::mouseReleaseEvent(event);
}

void MindmapNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit nodeSelected(this);
}

void MindmapNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    emit nodeDoubleClick(this);
}
