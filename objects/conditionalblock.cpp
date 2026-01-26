#include "conditionalblock.h"
#include "connectionpoint.h"

#include <algorithm>
#include <QPainter>

namespace {
    constexpr float CornerOffset = 22.5f;
}

ConditionalBlock::ConditionalBlock(QGraphicsObject *parent)
    : ObjectBase(parent)
{
    m_label = "...";
    m_color = QColor(255, 255, 255);
    m_size = QSizeF(45, 45);

    createConnectionPoint(&m_topConnection, Qt::AlignTop);
    createConnectionPoint(&m_rightConnection, Qt::AlignRight);
    createConnectionPoint(&m_leftConnection, Qt::AlignLeft);
}

void ConditionalBlock::paintBlock(QPainter *painter)
{
    painter->setPen(QPen(Qt::black, 1.2));
    painter->setBrush(m_color);

    auto [w, h] = std::pair{ m_size.width(), m_size.height()};
    auto points = translateRotate(-w/2, -h/2, w, h);

    painter->drawPolygon(points.data(), points.size());
}

QPainterPath ConditionalBlock::blockShape() const
{
    QPainterPath path;

    auto [w, h] = std::pair{ m_size.width() + 8, m_size.height() + 8};
    path.addRect(-w/2, -h/2, w, h);
    return path;
}

std::array<QPointF, 6> ConditionalBlock::translateRotate(float x, float y, float width, float height) {
    QPointF outerLeft = QPointF(x, y + height/2);
    QPointF top = QPointF(x + CornerOffset, y);
    QPointF right = QPointF(std::max(x + width - CornerOffset, x + width/2), y);
    QPointF outerRight = QPointF(x + width, y + height/2);
    QPointF bottom = QPointF(x + width - CornerOffset, y + height);
    QPointF left = QPointF(std::min(x + width/2, x + CornerOffset), y + height);

    return {outerLeft, top, right, outerRight, bottom, left};
}
