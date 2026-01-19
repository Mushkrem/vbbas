#include "objectbase.h"
#include "connectionpoint.h"

#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QJsonObject>
#include <QUuid>

ObjectBase::ObjectBase(QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , m_id(QUuid::createUuid().toString(QUuid::WithoutBraces))
    , m_label("Block")
    , m_color(Qt::lightGray)
    , m_size(100, 60)
{
    setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
}

ObjectBase::~ObjectBase() {}

QJsonObject ObjectBase::toJson() const
{
    QJsonObject json;
    json["id"] = m_id;
    json["type"] = static_cast<int>(blockType());
    json["label"] = m_label;
    json["color"] = m_color.name();
    json["x"] = pos().x();
    json["y"] = pos().y();
    return json;
}

void ObjectBase::fromJson(const QJsonObject &json)
{
    m_id = json["id"].toString();
    m_label = json["label"].toString();
    m_color = QColor(json["color"].toString());
    setPos(json["x"].toDouble(), json["y"].toDouble());
}

void ObjectBase::setLabel(const QString &label)
{
    m_label = label;
    update();
}

void ObjectBase::setColor(const QColor &color)
{
    m_color = color;
    update();
}

QList<ConnectionPoint*> ObjectBase::connectionPoints() const
{
    QList<ConnectionPoint*> points;
    if (m_topConnection) points.append(m_topConnection);
    if (m_rightConnection) points.append(m_rightConnection);
    if (m_bottomConnection) points.append(m_bottomConnection);
    if (m_leftConnection) points.append(m_leftConnection);
    return points;
}

void ObjectBase::createConnectionPoint(ConnectionPoint **point, Qt::Alignment position)
{
    *point = new ConnectionPoint(position, this);
}

QRectF ObjectBase::boundingRect() const
{
    return QRectF(-m_size.width() / 2, -m_size.height() / 2,
                  m_size.width(), m_size.height()).adjusted(-5, -5, 5, 5);
}

void ObjectBase::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);

    if (isSelected()) {
        painter->setPen(QPen(Qt::blue, 2, Qt::DashLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawPath(shape());
    }

    paintBlock(painter);

    painter->setPen(Qt::black);
    painter->setFont(QFont("Arial", 10));
    painter->drawText(boundingRect(), Qt::AlignCenter, m_label);
}

QPainterPath ObjectBase::shape() const
{
    return blockShape();
}

void ObjectBase::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void ObjectBase::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
    for (ConnectionPoint *point : connectionPoints()) {
        point->updateConnections();
    }
}

void ObjectBase::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}

void ObjectBase::mouseRightClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    // todo
}

void ObjectBase::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    // todo
}
