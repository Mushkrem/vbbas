#ifndef STATEMENTBLOCK_H
#define STATEMENTBLOCK_H

#include "objectbase.h"

class StatementBlock : public ObjectBase
{
public:
    explicit StatementBlock(QGraphicsObject *parent = nullptr);

    ObjectActions::ObjectType blockType() const override { return ObjectActions::ObjectType::Statement; }

protected:
    void paintBlock(QPainter *painter) override;
    QPainterPath blockShape() const override;
};

#endif // STATEMENTBLOCK_H
