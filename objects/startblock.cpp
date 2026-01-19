#include "startblock.h"
#include "connectionpoint.h"

#include <QPainter>

StartBlock::StartBlock(QGraphicsItem *parent)
    : ObjectBase(parent)
{
    m_label = "Start";
    m_color = QColor(200, 255, 170); // green
    m_size = QSizeF(80, 80);

    createConnectionPoint(&m_bottomConnection, Qt::AlignBottom);
}

void StartBlock::paintBlock(QPainter *painter)
{
    painter->setPen(QPen(Qt::black, 1.2));
    painter->setBrush(m_color);

    qreal radius = m_size.width() / 2;
    painter->drawEllipse(QPointF(0, 0), radius, radius/1.72);
}

QPainterPath StartBlock::blockShape() const
{
    QPainterPath path;
    qreal radius = m_size.width() / 2;
    path.addRect(-radius, -(radius/1.72), radius * 2, (radius/1.72) * 2);
    return path;
}
