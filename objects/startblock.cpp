#include "startblock.h"
#include "connectionpoint.h"

#include <QPainter>

StartBlock::StartBlock(QGraphicsObject *parent)
    : ObjectBase(parent)
{
    m_label = "START";
    m_color = QColor(255, 255, 255);
    m_size = QSizeF(75, 75/1.72);

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
    auto [w, h] = std::pair{m_size.width() + 8, m_size.width()/1.72 + 8};
    path.addRect(-w/2, -h/2, w, h);
    return path;
}
