#ifndef OBJECTTYPES_H
#define OBJECTTYPES_H

#include <QObject>

namespace ObjectTypes {
    Q_NAMESPACE

    enum class BlockType {
        Start,
        Stop,
        Event,
        Statement,
        Conditional,
        Interaction
    };
    Q_ENUM_NS(BlockType)
}

#endif // OBJECTTYPES_H
