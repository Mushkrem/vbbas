#ifndef DOCUMENTSMANAGER_H
#define DOCUMENTSMANAGER_H

#include "idocumentinfo.h"
#include "documenttab.h"

#include <QTabWidget>
#include <QObject>
#include <QLabel>

class DocumentsManager : public QObject, public IDocumentInfo {
    Q_OBJECT
public:
    explicit DocumentsManager(QTabWidget *tabWidget, QObject *parent = nullptr);
    DocumentTab *currentDocument() const {
        return m_documents.value(m_tabWidget->currentIndex(), nullptr); // For some reason returns THE PREVIOUS tab. Bug
    };

    QString currentDocumentName() const override;
    bool currentDocumentModified() const override;
    bool hasOpenDocuments() const override;
    int documentCount() const override;

public slots:
    void createNewDocument();
    void openDocument(const QString &path);
    void saveCurrentDocument();
    void saveDocument(DocumentTab *document);
    void closeDocument();
    void closeDocument(int index);

signals:
    void documentCreated(DocumentTab *document);
    void documentOpened(DocumentTab *document);
    void documentClosed(DocumentTab *document);
    void documentModificationChanged(bool modified);

private:
    QTabWidget *m_tabWidget;
    QList<DocumentTab*> m_documents;
    QHash<DocumentTab*, QLabel*> m_labels;
};

#endif // DOCUMENTSMANAGER_H
