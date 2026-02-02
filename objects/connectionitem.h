#ifndef CONNECTIONITEM_H
#define CONNECTIONITEM_H

#include <QGraphicsPathItem>
#include <QJsonObject>

class ConnectionPoint;

class ConnectionItem : public QGraphicsPathItem
{
public:
    explicit ConnectionItem(ConnectionPoint *startPoint, ConnectionPoint *endPoint, QGraphicsItem *parent = nullptr);
    ~ConnectionItem();

    ConnectionPoint* startPoint() const { return m_startPoint; }
    ConnectionPoint* endPoint() const { return m_endPoint; }

    void setEndPoint(ConnectionPoint *endPoint);
    void setTemporaryEndPoint(const QPointF &scenePos);

    void updatePath();

    QJsonObject toJson() const;
    static ConnectionItem* fromJson(const QJsonObject &json, QGraphicsScene *scene);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QPainterPath createPath(const QPointF &start, const QPointF &end) const;

    ConnectionPoint *m_startPoint;
    ConnectionPoint *m_endPoint;
    QPointF m_temporaryEndPoint;
    bool m_isTemporary = false;
};

#endif // CONNECTIONITEM_H
