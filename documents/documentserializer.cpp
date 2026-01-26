#include "documentserializer.h"
#include "documenttab.h"

#include "../actions/objectactions.h"

QJsonObject DocumentSerializer::toJson(const DocumentTab *document) {
    QJsonObject json;
    json["fileFormat"] = "vibas-algorithm";
    json["version"] = currentVersion();

    json["metadata"] = serializeMetadata(document);

    QJsonObject canvas;
    canvas["gridSize"] = 20;
    canvas["showGrid"] = true;
    canvas["backgroundColor"] = "#FFFFFF";
    json["canvas"] = canvas;

    json["blocks"] = serializeBlocks(document);
    json["variables"] = serializeVariables(document);
    json["connections"] = serializeConnections(document);

    return json;
}

QJsonObject DocumentSerializer::serializeMetadata(const DocumentTab *document) {
    QJsonObject metadata;
    metadata["title"] = document->title();

    return metadata;
}

QJsonArray DocumentSerializer::serializeBlocks(const DocumentTab *document) {
    QJsonArray blocksArray;
    QList<ObjectBase*> blocks = document->blocks();

    for (ObjectBase *block : std::as_const(blocks)) {
        if (block)
            blocksArray.append(block->toJson());
    }

    return blocksArray;
}

QJsonArray DocumentSerializer::serializeConnections(const DocumentTab *document) {
    QJsonArray connectionsArray;
    QList<ConnectionItem*> connections = document->connections();

    for (ConnectionItem *connection : std::as_const(connections)) {
        if (connection)
            connectionsArray.append(connection->toJson());
    }

    return connectionsArray;
}

QJsonArray DocumentSerializer::serializeVariables(const DocumentTab *document) {
    QJsonArray variables;
    return variables;
}

bool DocumentSerializer::fromJson(DocumentTab *document, const QJsonObject &json, QString &error) {
    if (json["fileFormat"].toString() != "vibas-algorithm") {
        error = "Invalid file format";
        return false;
    }

    QString version = json["version"].toString();
    if (version != currentVersion()) {
        // messagebox
    }

    document->scene()->clear();

    if (!deserializeMetadata(document, json["metadata"].toObject())) {
        error = "Failed to load metadata of the file. Step (1/4)";
        return false;
    }

    QHash<QString, ObjectBase*> blockMap = deserializeBlocks(document, json["blocks"].toArray());
    if (blockMap.isEmpty() && !json["blocks"].toArray().isEmpty()) {
        error = "Failed to load blocks. Step (2/4)";
        return false;
    }

    if (!deserializeConnections(document, json["connections"].toArray(), blockMap)) {
        error = "Failed to load connections. Step (3/4)";
        return false;
    }

    if (!deserializeVariables(document, json["variables"].toArray())) {
        error = "Failed to load variables. Step (4/4)";
        return false;
    }

    return true;
}

bool DocumentSerializer::deserializeMetadata(DocumentTab *document, const QJsonObject &json) {
    QString title = json["title"].toString();
    if (title.isEmpty())
        return false;

    document->setTitle(title);

    return true;
}

QHash<QString, ObjectBase*> DocumentSerializer::deserializeBlocks(DocumentTab *document, const QJsonArray &blocks) {
    QHash<QString, ObjectBase*> blockMap;

    for (const QJsonValue &value : blocks) {
        QJsonObject blockJson = value.toObject();

        int typeInt = blockJson["type"].toInt();
        ObjectActions::ObjectType type = static_cast<ObjectActions::ObjectType>(typeInt);

        ObjectBase *block = ObjectFactory::createBlock(type);
        if (!block) {
            qDebug() << "Failed for type:" << typeInt;
            continue;
        }

        block->fromJson(blockJson);

        QPointF pos(blockJson["x"].toDouble(), blockJson["y"].toDouble());
        document->addBlock(block, pos);

        blockMap.insert(block->id(), block);
    }

    return blockMap;
}
bool DocumentSerializer::deserializeConnections(DocumentTab *document, const QJsonArray &connections, const QHash<QString, ObjectBase*> &blockMap) {
    for (const QJsonValue &value : connections) {
        QJsonObject connJson = value.toObject();

        QString startBlockId = connJson["startBlockId"].toString();
        QString endBlockId = connJson["endBlockId"].toString();

        ObjectBase *startBlock = blockMap.value(startBlockId);
        ObjectBase *endBlock = blockMap.value(endBlockId);

        if (!startBlock || !endBlock) {
            qDebug() << "Blocks for connection do not exist:" << startBlockId << "->" << endBlockId;
            continue;
        }

        Qt::Alignment startPos = static_cast<Qt::Alignment>(connJson["startPosition"].toInt());
        Qt::Alignment endPos = static_cast<Qt::Alignment>(connJson["endPosition"].toInt());

        ConnectionPoint *startPoint = findConnectionPoint(startBlock, startPos);
        ConnectionPoint *endPoint = findConnectionPoint(endBlock, endPos);

        if (!startPoint || !endPoint) {
            continue;
        }

        ConnectionItem *connection = new ConnectionItem(startPoint, endPoint);
        document->addConnection(connection);
    }

    return true;
}

bool DocumentSerializer::deserializeVariables(DocumentTab *document, const QJsonArray &variables) {
    Q_UNUSED(document);
    Q_UNUSED(variables);
    return true;
}

ConnectionPoint* DocumentSerializer::findConnectionPoint(ObjectBase *block, Qt::Alignment position)
{
    if (position & Qt::AlignTop)
        return block->topConnection();
    else if (position & Qt::AlignBottom)
        return block->bottomConnection();
    else if (position & Qt::AlignLeft)
        return block->leftConnection();
    else if (position & Qt::AlignRight)
        return block->rightConnection();

    return nullptr;
}

