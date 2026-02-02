#ifndef CONNECTIONPOINT_H
#define CONNECTIONPOINT_H

#include <QGraphicsEllipseItem>
#include <QList>

class ConnectionItem;
class ObjectBase;

class ConnectionPoint : public QGraphicsEllipseItem
{
public:
    explicit ConnectionPoint(Qt::Alignment position, ObjectBase *parent);

    Qt::Alignment position() const { return m_position; }
    ObjectBase* parentBlock() const { return m_parentBlock; }

    void addConnection(ConnectionItem *connection);
    void removeConnection(ConnectionItem *connection);
    QList<ConnectionItem*> connections() const { return m_connections; }

    void updateConnections();

    QPointF sceneConnectionPoint() const;

    static ConnectionPoint* findNearestConnectionPoint(QGraphicsScene *scene, const QPointF &scenePos, ConnectionPoint *exclude, qreal maxDistance = 30.0);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    void updatePosition();

    Qt::Alignment m_position;
    ObjectBase *m_parentBlock;
    QList<ConnectionItem*> m_connections;

    ConnectionItem *m_temporaryConnection = nullptr;
    bool m_isHovered = false;
};

#endif // CONNECTIONPOINT_H
