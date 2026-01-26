#ifndef STARTBLOCK_H
#define STARTBLOCK_H

#include "objectbase.h"

class StartBlock : public ObjectBase
{
public:
    explicit StartBlock(QGraphicsObject *parent = nullptr);

    ObjectActions::ObjectType blockType() const override { return ObjectActions::ObjectType::Start; }

protected:
    void paintBlock(QPainter *painter) override;
    QPainterPath blockShape() const override;
};

#endif // STARTBLOCK_H
