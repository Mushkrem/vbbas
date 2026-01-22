#include "objectfactory.h"
#include "interactionblock.h"
#include "conditionalblock.h"
#include "statementblock.h"
#include "startblock.h"
#include "stopblock.h"

ObjectBase* ObjectFactory::createBlock(ObjectActions::ObjectType type)
{
    switch (type) {
    case ObjectActions::ObjectType::Start:
        return new StartBlock();
    case ObjectActions::ObjectType::Stop:
        return new StopBlock();
    case ObjectActions::ObjectType::Statement:
        return new StatementBlock();
    case ObjectActions::ObjectType::Conditional:
        return new ConditionalBlock();
    case ObjectActions::ObjectType::Interaction:
        return new InteractionBlock();
    default:
        return nullptr;
    }
}
