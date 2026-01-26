#ifndef DOCUMENTSERIALIZER_H
#define DOCUMENTSERIALIZER_H

#include <QJsonObject>
#include <QJsonArray>

#include "../objects/connectionitem.h"
#include "../objects/objectfactory.h"

class DocumentTab;

class DocumentSerializer
{
public:
    static QJsonObject toJson(const DocumentTab *document);
    static bool fromJson(DocumentTab *document, const QJsonObject &json, QString &error);
    static QString currentVersion() { return "1.0"; }

private:
    static QJsonObject serializeMetadata(const DocumentTab *document);
    static QJsonArray serializeBlocks(const DocumentTab *document);
    static QJsonArray serializeVariables(const DocumentTab *document);
    static QJsonArray serializeConnections(const DocumentTab *document);

    static bool deserializeMetadata(DocumentTab *document, const QJsonObject &json);
    static QHash<QString, ObjectBase*> deserializeBlocks(DocumentTab *document, const QJsonArray &blocks);
    static bool deserializeVariables(DocumentTab *document, const QJsonArray &variables);
    static bool deserializeConnections(DocumentTab *document, const QJsonArray &connections, const QHash<QString, ObjectBase*> &blockMap);

    static ConnectionPoint* findConnectionPoint(ObjectBase *block, Qt::Alignment position);
};

#endif // DOCUMENTSERIALIZER_H
