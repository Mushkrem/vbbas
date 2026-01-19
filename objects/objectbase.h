#ifndef OBJECTBASE_H
#define OBJECTBASE_H

#include "../actions/objectactions.h"

#include <QGraphicsItem>
#include <QList>
#include <QUuid>

class ConnectionPoint;
class ConnectionItem;

class ObjectBase : public QGraphicsItem
{
public:
    explicit ObjectBase(QGraphicsItem *parent = nullptr);
    virtual ~ObjectBase();

    QString id() const { return m_id; }
    virtual ObjectActions::ObjectType blockType() const = 0;

    virtual QJsonObject toJson() const;
    virtual void fromJson(const QJsonObject &json);

    ConnectionPoint* topConnection() const { return m_topConnection; }
    ConnectionPoint* rightConnection() const { return m_rightConnection; }
    ConnectionPoint* bottomConnection() const { return m_bottomConnection; }
    ConnectionPoint* leftConnection() const { return m_leftConnection; }

    QList<ConnectionPoint*> connectionPoints() const;

    QString label() const { return m_label; }
    void setLabel(const QString &label);

    QColor color() const { return m_color; }
    void setColor(const QColor &color);

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QPainterPath shape() const override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseRightClickEvent(QGraphicsSceneMouseEvent *event);

    virtual void paintBlock(QPainter *painter) = 0;
    virtual QPainterPath blockShape() const = 0;

    void createConnectionPoint(ConnectionPoint **point, Qt::Alignment position);

    QString m_id;
    QString m_label;
    QColor m_color;
    QSizeF m_size;

    ConnectionPoint *m_topConnection = nullptr;
    ConnectionPoint *m_rightConnection = nullptr;
    ConnectionPoint *m_bottomConnection = nullptr;
    ConnectionPoint *m_leftConnection = nullptr;
};

#endif // OBJECTBASE_H
