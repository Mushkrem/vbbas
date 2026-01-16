#ifndef FILESERVICE_H
#define FILESERVICE_H

#include <QString>
#include <QJsonObject>

class DocumentTab;

class FileService
{
public:
    static bool saveDocument(DocumentTab *document, const QString &filePath, QString &error);
    static bool loadDocument(DocumentTab *document, const QString &filePath, QString &error);
    static bool isValidFile(DocumentTab *document, const QString &filePath);

    static QString fileExtension() { return "vib";  }
    static QString fileFilter() { return "Algorithm Files (*.vib)";  }

private:
    static bool writeJsonToFile(const QString &filePath, const QJsonObject &json, QString &error);
    static bool readJsonFromFile(const QString &filePath, QJsonObject &json, QString &error);
};

#endif // FILESERVICE_H
