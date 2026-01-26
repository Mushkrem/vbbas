#ifndef STOPBLOCK_H
#define STOPBLOCK_H

#include "objectbase.h"

class StopBlock : public ObjectBase
{
public:
    explicit StopBlock(QGraphicsObject *parent = nullptr);

    ObjectActions::ObjectType blockType() const override { return ObjectActions::ObjectType::Stop; }

protected:
    void paintBlock(QPainter *painter) override;
    QPainterPath blockShape() const override;
};

#endif // STOPBLOCK_H
