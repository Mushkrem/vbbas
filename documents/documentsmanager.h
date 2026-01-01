#ifndef DOCUMENTSMANAGER_H
#define DOCUMENTSMANAGER_H

#include "documenttab.h"
#include <QTabWidget>
#include <QObject>

class DocumentsManager : public QObject {
    Q_OBJECT
public:
    explicit DocumentsManager(QTabWidget *tabWidget, QObject *parent = nullptr);

public slots:
    void createNewDocument();
    void openDocument(const QString &path);
    void saveDocument(DocumentTab *document);
    void closeDocument();
    void closeDocument(int index);

signals:
    void documentCreated(DocumentTab *document);
    void documentOpened(DocumentTab *document);
    void documentClosed(DocumentTab *document);

private:
    QTabWidget *m_tabWidget;
    QList<DocumentTab*> m_documents;
};

#endif // DOCUMENTSMANAGER_H
