#include "objectfactory.h"
#include "startblock.h"

ObjectBase* ObjectFactory::createBlock(ObjectActions::ObjectType type)
{
    switch (type) {
    case ObjectActions::ObjectType::Start:
        return new StartBlock();
    default:
        return nullptr;
    }
}
