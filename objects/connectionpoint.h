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

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    void updatePosition();

    Qt::Alignment m_position;
    ObjectBase *m_parentBlock;
    QList<ConnectionItem*> m_connections;
};

#endif // CONNECTIONPOINT_H
