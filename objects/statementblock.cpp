#include "statementblock.h"
#include "connectionpoint.h"

#include <QPainter>

StatementBlock::StatementBlock(QGraphicsItem *parent)
    : ObjectBase(parent)
{
    m_label = "...";
    m_color = QColor(255, 255, 255);
    m_size = QSizeF(75, 37.5);

    createConnectionPoint(&m_topConnection, Qt::AlignTop);
    createConnectionPoint(&m_rightConnection, Qt::AlignRight);
    createConnectionPoint(&m_bottomConnection, Qt::AlignBottom);
    createConnectionPoint(&m_leftConnection, Qt::AlignLeft);
}

void StatementBlock::paintBlock(QPainter *painter)
{
    painter->setPen(QPen(Qt::black, 1));
    painter->setBrush(m_color);

    auto [w, h] = std::pair{ m_size.width(), m_size.height()};
    painter->drawRect(-w/2, -h/2, w, h);
}

QPainterPath StatementBlock::blockShape() const
{
    QPainterPath path;

    auto [w, h] = std::pair{ m_size.width() + 8, m_size.height() + 8};
    path.addRect(-w/2, -h/2, w, h);
    return path;
}
