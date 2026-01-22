#include "interactionblock.h"
#include "connectionpoint.h"

#include <QPainter>

InteractionBlock::InteractionBlock(QGraphicsItem *parent)
    : ObjectBase(parent)
{
    m_label = "...";
    m_color = QColor(255, 255, 255);
    m_size = QSizeF(75, 37.5);

    createConnectionPoint(&m_topConnection, Qt::AlignTop);
    createConnectionPoint(&m_bottomConnection, Qt::AlignBottom);
}

void InteractionBlock::paintBlock(QPainter *painter)
{
    painter->setPen(QPen(Qt::black, 1.2));
    painter->setBrush(m_color);

    auto [w, h] = std::pair{ m_size.width(), m_size.height()};
    auto points = translateX(-w/2, -h/2, w, h);

    painter->drawPolygon(points.data(), points.size());
}

QPainterPath InteractionBlock::blockShape() const
{
    QPainterPath path;

    auto [w, h] = std::pair{ m_size.width() + 8, m_size.height() + 8};
    path.addRect(-w/2, -h/2, w, h);
    return path;
}

std::array<QPointF, 4> InteractionBlock::translateX(float x, float y, float width, float height) {
    QPointF top = QPointF(x + width/8, y);
    QPointF right = QPointF(x + width, y);
    QPointF bottom = QPointF(x + width - width/8, y + height);
    QPointF left = QPointF(x, y + height);

    return {top, right, bottom, left};
}
