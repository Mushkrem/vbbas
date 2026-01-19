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

void ConnectionItem::updatePath()
{
    if (!m_startPoint || !m_endPoint) {
        return;
    }

    QPointF start = m_startPoint->sceneConnectionPoint();
    QPointF end = m_endPoint->sceneConnectionPoint();

    QPainterPath path;
    path.moveTo(mapFromScene(start));

    // smooth path
    qreal dx = end.x() - start.x();
    qreal dy = end.y() - start.y();

    // points bezier curve
    QPointF c1 = mapFromScene(start + QPointF(dx * 0.5, 0));
    QPointF c2 = mapFromScene(end - QPointF(dx * 0.5, 0));

    path.cubicTo(c1, c2, mapFromScene(end));

    setPath(path);
}

void ConnectionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(pen());
    painter->drawPath(path());

    // draw arrow at end
    QPointF end = path().pointAtPercent(1.0);
    QPointF direction = path().pointAtPercent(0.95) - end;
    qreal angle = std::atan2(-direction.y(), -direction.x());

    QPointF arrowP1 = end + QPointF(std::sin(angle + M_PI / 3) * 10,
                                    std::cos(angle + M_PI / 3) * 10);
    QPointF arrowP2 = end + QPointF(std::sin(angle + M_PI - M_PI / 3) * 10,
                                    std::cos(angle + M_PI - M_PI / 3) * 10);

    QPainterPath arrowHead;
    arrowHead.moveTo(end);
    arrowHead.lineTo(arrowP1);
    arrowHead.lineTo(arrowP2);
    arrowHead.closeSubpath();

    painter->setBrush(pen().color());
    painter->drawPath(arrowHead);
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
