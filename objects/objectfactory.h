#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include "../actions/objectactions.h"

class ObjectBase;

class ObjectFactory
{
public:
    static ObjectBase* createBlock(ObjectActions::ObjectType type);
};

#endif // OBJECTFACTORY_H
