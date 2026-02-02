#include "connectionpoint.h"
#include "connectionitem.h"
#include "objectbase.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>

ConnectionPoint::ConnectionPoint(Qt::Alignment position, ObjectBase *parent)
    : QGraphicsEllipseItem(-4, -4, 8, 8, parent)
    , m_position(position)
    , m_parentBlock(parent)
{
    setBrush(Qt::black);
    setPen(QPen(Qt::white, 1));
    setZValue(10);
    setAcceptHoverEvents(true);
    setCursor(Qt::CrossCursor);

    updatePosition();
}

void ConnectionPoint::updatePosition()
{
    QRectF parentRect = static_cast<QGraphicsItem*>(m_parentBlock)->boundingRect();
    QPointF newPos;

    if (m_position & Qt::AlignTop) {
        newPos = QPointF(parentRect.center().x(), parentRect.top());
    } else if (m_position & Qt::AlignBottom) {
        newPos = QPointF(parentRect.center().x(), parentRect.bottom());
    } else if (m_position & Qt::AlignLeft) {
        newPos = QPointF(parentRect.left(), parentRect.center().y());
    } else if (m_position & Qt::AlignRight) {
        newPos = QPointF(parentRect.right(), parentRect.center().y());
    }

    setPos(newPos);
}

QPointF ConnectionPoint::sceneConnectionPoint() const
{
    return mapToScene(rect().center());
}

void ConnectionPoint::addConnection(ConnectionItem *connection)
{
    if (!m_connections.contains(connection)) {
        m_connections.append(connection);
    }
}

void ConnectionPoint::removeConnection(ConnectionItem *connection)
{
    m_connections.removeOne(connection);
}

void ConnectionPoint::updateConnections()
{
    updatePosition();
    for (ConnectionItem *connection : std::as_const(m_connections)) {
        connection->updatePath();
    }
}

void ConnectionPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // painter->setRenderHint(QPainter::Antialiasing);
    // painter->setBrush(brush());
    // painter->setPen(pen());
    // painter->drawEllipse(rect());
}

ConnectionPoint* ConnectionPoint::findNearestConnectionPoint(QGraphicsScene *scene, const QPointF &scenePos, ConnectionPoint *exclude, qreal maxDistance)
{
    ConnectionPoint *nearest = nullptr;
    qreal minDistance = maxDistance;

    QList<QGraphicsItem*> items = scene->items();
    for (QGraphicsItem *item : std::as_const(items)) {
        ConnectionPoint *point = qgraphicsitem_cast<ConnectionPoint*>(item);
        if (!point || point == exclude) {
            continue;
        }

        if (exclude && point->parentBlock() == exclude->parentBlock()) {
            continue;
        }

        qreal distance = QLineF(scenePos, point->sceneConnectionPoint()).length();
        if (distance < minDistance) {
            minDistance = distance;
            nearest = point;
        }
    }

    return nearest;
}

void ConnectionPoint::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_temporaryConnection = new ConnectionItem(this, nullptr);
        scene()->addItem(m_temporaryConnection);

        event->accept();
    } else {
        QGraphicsEllipseItem::mousePressEvent(event);
    }
}

void ConnectionPoint::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_temporaryConnection) {
        m_temporaryConnection->setTemporaryEndPoint(event->scenePos());
        event->accept();
    } else {
        QGraphicsEllipseItem::mouseMoveEvent(event);
    }
}

void ConnectionPoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_temporaryConnection) {
        ConnectionPoint *targetPoint = findNearestConnectionPoint(
            scene(),
            event->scenePos(),
            this,
            30.0
            );

        if (targetPoint) {
            m_temporaryConnection->setEndPoint(targetPoint);
        } else {
            scene()->removeItem(m_temporaryConnection);
            delete m_temporaryConnection;
            m_temporaryConnection = nullptr;
        }

        m_temporaryConnection = nullptr;
        event->accept();
    } else {
        QGraphicsEllipseItem::mouseReleaseEvent(event);
    }
}

void ConnectionPoint::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    m_isHovered = true;
    update();
}

void ConnectionPoint::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    m_isHovered = false;
    update();
}
