#include "fileservice.h"
#include "documentserializer.h"
#include "documenttab.h"

#include <QFileInfo>
#include <QFile>

bool FileService::saveDocument(DocumentTab *document, const QString &filePath, QString &error) {
    if (!document) {
        error = "Invalid document.";
        return false;
    }

    // Serialize document to JSON
    QJsonObject json = DocumentSerializer::toJson(document);

    // Write to file
    if (!writeJsonToFile(filePath, json, error))
        return false;

    QFileInfo fileInfo(filePath);
    document->setTitle(fileInfo.fileName());

    return true;
}

bool FileService::loadDocument(DocumentTab *document, const QString &filePath, QString &error) {
    if (!document) {
        error = "Invalid document.";
        return false;
    }

    // Read JSON from file
    QJsonObject json;
    if (!readJsonFromFile(filePath, json, error))
        return false;

    // Deserialize JSON to document
    if (!DocumentSerializer::fromJson(document, json, error))
        return false;

    return true;
}

bool FileService::isValidFile(DocumentTab *document, const QString &filePath) {
    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists() || !fileInfo.isFile())
        return false;

    if (fileInfo.suffix().toLower() != fileExtension())
        return false;

    return true;
}

bool FileService::writeJsonToFile(const QString &filePath, const QJsonObject &json, QString &error) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        error = "Could not open file for writing: " + file.errorString();
        return false;
    }

    QJsonDocument doc(json);
    qint64 bytesWritten = file.write(doc.toJson(QJsonDocument::Indented));

    if (bytesWritten == -1) {
        error = "Failed to write to file: " + file.errorString();
        file.close();
        return false;
    }

    file.close();
    return true;
}

bool FileService::readJsonFromFile(const QString &filePath, QJsonObject &json, QString &error) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        error = "Could not open file for reading: " + file.errorString();
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        error = "JSON parse error: " + parseError.errorString();
        return false;
    }

    if (!doc.isObject()) {
        error = "Invalid JSON structure.";
        return false;
    }

    json = doc.object();
    return true;
}
