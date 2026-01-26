#ifndef INTERACTIONBLOCK_H
#define INTERACTIONBLOCK_H

#include "objectbase.h"

class InteractionBlock : public ObjectBase
{
public:
    explicit InteractionBlock(QGraphicsObject *parent = nullptr);

    ObjectActions::ObjectType blockType() const override { return ObjectActions::ObjectType::Interaction; }

protected:
    void paintBlock(QPainter *painter) override;
    QPainterPath blockShape() const override;

private:
    std::array<QPointF, 4> translateX(float x, float y, float width, float height);
};
#endif // INTERACTIONBLOCK_H
