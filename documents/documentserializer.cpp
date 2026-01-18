#include "documentserializer.h"
#include "documenttab.h"

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
    QJsonArray blocks;
    return blocks;
}

QJsonArray DocumentSerializer::serializeVariables(const DocumentTab *document) {
    QJsonArray variables;
    return variables;
}

QJsonArray DocumentSerializer::serializeConnections(const DocumentTab *document) {
    QJsonArray connections;
    return connections;
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

    if (!deserializeMetadata(document, json["metadata"].toObject()) && error.isEmpty()) {
        error = "Failed to load metadata of the file. Step (1/4)";
        return false;
    }

    if (!deserializeBlocks(document, json["blocks"].toArray()) && error.isEmpty()) {
        error = "Failed to load blocks. Step (2/4)";
        return false;
    }

    if (!deserializeConnections(document, json["connections"].toArray()) && error.isEmpty()) {
        error = "Failed to load connections. Step (3/4)";
        return false;
    }

    if (!deserializeVariables(document, json["variables"].toArray()) && error.isEmpty()) {
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
bool DocumentSerializer::deserializeBlocks(DocumentTab *document, const QJsonArray &blocks) {
    Q_UNUSED(document);
    Q_UNUSED(blocks);

    return true;
}
bool DocumentSerializer::deserializeVariables(DocumentTab *document, const QJsonArray &variables) {
    Q_UNUSED(document);
    Q_UNUSED(variables);

    return true;
}
bool DocumentSerializer::deserializeConnections(DocumentTab *document, const QJsonArray &connections) {
    Q_UNUSED(document);
    Q_UNUSED(connections);

    return true;
}

