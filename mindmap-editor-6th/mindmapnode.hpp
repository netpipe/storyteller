#ifndef MINDMAPNODE_HPP
#define MINDMAPNODE_HPP

#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>
#include <QFont>

class MindmapNode: public QObject, public QGraphicsRectItem
{
    Q_OBJECT
private:
    size_t _nodeId;
    QString _content;
    QGraphicsTextItem *_textContainer;
    QBrush _brush;
    QPen _pen;
    QFont _font;
    QStringList _yes;
    QStringList _no;
    QStringList _maybe;
    QString _description;
//    QString type;
//    QString choice;

public:
    MindmapNode(size_t nodeId,
                const QString& content,
                const QBrush& brush,
                const QPen& pen,
                const QFont& font);

    size_t getNodeId() const;
    void setNodeId(const size_t& id);

    QJsonValue toJSON() const;
    void fromJSON(const QJsonObject& jsonObject,int,int);
    void setContent(const QString& content);
    QString getContent() const;
    void setYesList(const QStringList& yesList);
    QStringList getYesList() const;
    void setNoList(const QStringList& noList);
    QStringList getNoList() const;
    void setMaybeList(const QStringList& maybeList);
    QStringList getMaybeList() const;
    void setDescription(const QString& description);
    QString getDescription() const;

    void resize();

signals:
    void nodeSelected(MindmapNode* node);
    void positionChanged(MindmapNode* node);
    void nodeDoubleClick(MindmapNode* node);

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
};

Q_DECLARE_METATYPE(MindmapNode*)

#endif // MINDMAPNODE_HPP
