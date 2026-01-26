#ifndef CONDITIONALBLOCK_H
#define CONDITIONALBLOCK_H

#include "objectbase.h"

class ConditionalBlock : public ObjectBase
{
public:
    explicit ConditionalBlock(QGraphicsObject *parent = nullptr);

    ObjectActions::ObjectType blockType() const override { return ObjectActions::ObjectType::Conditional; }

protected:
    void paintBlock(QPainter *painter) override;
    QPainterPath blockShape() const override;

private:
    std::array<QPointF, 6> translateRotate(float x, float y, float width, float height);
};

#endif // CONDITIONALBLOCK_H
