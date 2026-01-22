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

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(brush());
    painter->setPen(pen());
    painter->drawEllipse(rect());
}

void ConnectionPoint::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << event->lastPos();
    m_parentBlock->setLabel("Hello!");
    event->accept();
}

void ConnectionPoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << event->lastPos();
    event->accept();
}
