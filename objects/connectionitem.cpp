#include "connectionitem.h"
#include "connectionpoint.h"
#include "objectbase.h"

#include <cmath>
#include <QPainter>
#include <QPainterPath>

ConnectionItem::ConnectionItem(ConnectionPoint *startPoint, ConnectionPoint *endPoint, QGraphicsItem *parent)
    : QGraphicsPathItem(parent)
    , m_startPoint(startPoint)
    , m_endPoint(endPoint)
{
    setPen(QPen(Qt::black, 2));
    setZValue(-1);

    if (m_startPoint) {
        m_startPoint->addConnection(this);
    }
    if (m_endPoint) {
        m_endPoint->addConnection(this);
        m_isTemporary = false;
    } else {
        m_isTemporary = true;
    }

    updatePath();
}

ConnectionItem::~ConnectionItem()
{
    if (m_startPoint) {
        m_startPoint->removeConnection(this);
    }
    if (m_endPoint) {
        m_endPoint->removeConnection(this);
    }
}

void ConnectionItem::setEndPoint(ConnectionPoint *endPoint)
{
    if (m_endPoint) {
        m_endPoint->removeConnection(this);
    }

    m_endPoint = endPoint;
    m_isTemporary = false;

    if (m_endPoint) {
        m_endPoint->addConnection(this);
    }

    updatePath();
}

void ConnectionItem::setTemporaryEndPoint(const QPointF &scenePos)
{
    m_temporaryEndPoint = scenePos;
    m_isTemporary = true;
    updatePath();
}

void ConnectionItem::updatePath()
{
    if (!m_startPoint) {
        return;
    }

    QPointF start = m_startPoint->sceneConnectionPoint();
    QPointF end;

    if (m_isTemporary) {
        end = m_temporaryEndPoint;
    } else if (m_endPoint) {
        end = m_endPoint->sceneConnectionPoint();
    } else {
        return;
    }

    QPainterPath path = createPath(start, end);
    setPath(path);
}

void ConnectionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);

    // Dashed line if temporary
    if (m_isTemporary) {
        painter->setPen(QPen(Qt::gray, 2, Qt::DashLine));
    } else {
        painter->setPen(pen());
    }

    painter->drawPath(path());

    // Draw arrow at end if not temporary
    if (!m_isTemporary && !path().isEmpty()) {
        QPointF start = path().pointAtPercent(0.0);
        QPointF afterStart = path().pointAtPercent(0.0);

        QPointF end = path().pointAtPercent(1.0);
        QPointF beforeEnd = path().pointAtPercent(0.95);

        qreal angle = std::atan2(end.y() - beforeEnd.y(), end.x() - beforeEnd.x());

        qreal arrowSize = 10;
        QPointF arrowP1 = end + QPointF(std::sin(angle + M_PI / 3) * arrowSize,
                                        std::cos(angle + M_PI / 3) * arrowSize);
        QPointF arrowP2 = end + QPointF(std::sin(angle + M_PI - M_PI / 3) * arrowSize,
                                        std::cos(angle + M_PI - M_PI / 3) * arrowSize);

        QPainterPath arrowHead;
        arrowHead.moveTo(end);
        arrowHead.lineTo(arrowP2);
        arrowHead.lineTo(arrowP1);
        arrowHead.closeSubpath();

        painter->setBrush(pen().color());
        painter->drawPath(arrowHead);
    }
}

// too complicated, needs a rework
// also buggy when connections are going out horizontally.
QPainterPath ConnectionItem::createPath(const QPointF &start, const QPointF &end) const
{
    QPainterPath path;
    path.moveTo(mapFromScene(start));

    QPointF localEnd = mapFromScene(end);
    QPointF localStart = mapFromScene(start);

    Qt::Alignment startDir = m_startPoint ? m_startPoint->position() : Qt::AlignRight;
    Qt::Alignment endDir = m_endPoint ? m_endPoint->position() : Qt::AlignLeft;

    bool useThreeBreakPoints = false;
    if (startDir & (Qt::AlignTop | Qt::AlignBottom)) {
        qreal verticalDistance = localEnd.y() - localStart.y();
        bool targetIsAbove = (startDir & Qt::AlignBottom) && (verticalDistance < 0);
        useThreeBreakPoints = targetIsAbove;
    } else if (startDir & (Qt::AlignLeft | Qt::AlignRight)) {
        qreal horizontalDistance = localEnd.x() - localStart.x();
        bool targetIsOpposite = (startDir & Qt::AlignRight && horizontalDistance < 0) ||
                                (startDir & Qt::AlignLeft && horizontalDistance > 0);
        useThreeBreakPoints = targetIsOpposite;
    }

    qreal minExtension;
    if (useThreeBreakPoints) {
        minExtension = 18.0;
    } else {
        if (startDir & (Qt::AlignTop | Qt::AlignBottom)) {
            qreal verticalDistance = std::abs(localEnd.y() - localStart.y());
            minExtension = qMax(18.0, verticalDistance * 0.5);
        } else {
            qreal horizontalDistance = std::abs(localEnd.x() - localStart.x());
            minExtension = qMax(18.0, horizontalDistance * 0.5);
        }
    }

    QPointF exitPoint = localStart;
    QPointF entryPoint = localEnd;

    if (startDir & Qt::AlignTop) {
        exitPoint.setY(localStart.y() - minExtension);
    } else if (startDir & Qt::AlignBottom) {
        exitPoint.setY(localStart.y() + minExtension);
    } else if (startDir & Qt::AlignLeft) {
        exitPoint.setX(localStart.x() - minExtension);
    } else if (startDir & Qt::AlignRight) {
        exitPoint.setX(localStart.x() + minExtension);
    }

    if (!m_isTemporary && m_endPoint) {
        if (endDir & Qt::AlignTop) {
            entryPoint.setY(localEnd.y() - minExtension);
        } else if (endDir & Qt::AlignBottom) {
            entryPoint.setY(localEnd.y() + minExtension);
        } else if (endDir & Qt::AlignLeft) {
            entryPoint.setX(localEnd.x() - minExtension);
        } else if (endDir & Qt::AlignRight) {
            entryPoint.setX(localEnd.x() + minExtension);
        }
    }
    path.lineTo(exitPoint);
    if (startDir & (Qt::AlignTop | Qt::AlignBottom)) {
        if (endDir & (Qt::AlignTop | Qt::AlignBottom)) {
            qreal verticalDistance = entryPoint.y() - exitPoint.y();
            bool targetIsAbove = (startDir & Qt::AlignBottom) && (verticalDistance < 0);
            bool targetIsTooClose = std::abs(verticalDistance) < minExtension * 2;

            if (targetIsAbove || targetIsTooClose) {
                qreal midX = (exitPoint.x() + entryPoint.x()) / 2;
                path.lineTo(midX, exitPoint.y());
                path.lineTo(midX, entryPoint.y());
            } else {
                path.lineTo(entryPoint.x(), exitPoint.y());
            }
        } else {
            path.lineTo(exitPoint.x(), entryPoint.y());
        }
    } else {
        if (endDir & (Qt::AlignLeft | Qt::AlignRight)) {
            qreal horizontalDistance = entryPoint.x() - exitPoint.x();
            bool targetIsOpposite = (startDir & Qt::AlignRight && horizontalDistance < 0) ||
                                    (startDir & Qt::AlignLeft && horizontalDistance > 0);
            bool targetIsTooClose = std::abs(horizontalDistance) < minExtension * 2;

            if (targetIsOpposite || targetIsTooClose) {
                qreal midY = (exitPoint.y() + entryPoint.y()) / 2;
                path.lineTo(exitPoint.x(), midY);
                path.lineTo(entryPoint.x(), midY);
            } else {
                path.lineTo(exitPoint.x(), entryPoint.y());
            }
        } else {
            path.lineTo(entryPoint.x(), exitPoint.y());
        }
    }

    path.lineTo(entryPoint);
    path.lineTo(localEnd);

    return path;
}


QJsonObject ConnectionItem::toJson() const
{
    QJsonObject json;

    if (m_startPoint && m_startPoint->parentBlock()) {
        json["startBlockId"] = m_startPoint->parentBlock()->id();
        json["startPosition"] = static_cast<int>(m_startPoint->position());
    }

    if (m_endPoint && m_endPoint->parentBlock()) {
        json["endBlockId"] = m_endPoint->parentBlock()->id();
        json["endPosition"] = static_cast<int>(m_endPoint->position());
    }

    return json;
}

ConnectionItem* ConnectionItem::fromJson(const QJsonObject &json, QGraphicsScene *scene)
{
    // todo deserialization. blocks by id (there is taht .type maybe ) and get their connection points
    Q_UNUSED(json);
    Q_UNUSED(scene);
    return nullptr;
}
