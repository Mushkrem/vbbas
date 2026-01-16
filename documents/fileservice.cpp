#include "fileservice.h"
#include "documentserializer.h"
#include "documenttab.h"

#include <QFileInfo>
#include <QFile>

bool FileService::saveDocument(DocumentTab *document, const QString &filePath, QString &error) {
    if (!document) {
        error = "Invalid document.";
        return EXIT_FAILURE;
    }

    // Serialize document to JSON
    QJsonObject json = DocumentSerializer::toJson(document);

    // Write to file
    if (!writeJsonToFile(filePath, json, error))
        return EXIT_FAILURE;

    QFileInfo fileInfo(filePath);
    document->setTitle(fileInfo.fileName());

    if (!error.isEmpty())
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

bool FileService::loadDocument(DocumentTab *document, const QString &filePath, QString &error) {
    if (!document) {
        error = "Invalid document.";
        return EXIT_FAILURE;
    }

    // Read JSON from file
    QJsonObject json;
    if (!readJsonFromFile(filePath, json, error))
        return EXIT_FAILURE;

    // Deserialize JSON to document
    if (!DocumentSerializer::fromJson(document, json, error))
        return EXIT_FAILURE;

    if (!error.isEmpty())
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
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
        return EXIT_FAILURE;
    }

    QJsonDocument doc(json);
    qint64 bytesWritten = file.write(doc.toJson(QJsonDocument::Indented));

    if (bytesWritten == -1) {
        error = "Failed to write to file: " + file.errorString();
        file.close();
        return EXIT_FAILURE;
    }

    file.close();
    return EXIT_SUCCESS;
}

bool FileService::readJsonFromFile(const QString &filePath, QJsonObject &json, QString &error) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        error = "Could not open file for reading: " + file.errorString();
        return EXIT_FAILURE;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        error = "JSON parse error: " + parseError.errorString();
        return EXIT_FAILURE;
    }

    if (!doc.isObject()) {
        error = "Invalid JSON structure.";
        return EXIT_FAILURE;
    }

    json = doc.object();
    return EXIT_SUCCESS;
}
