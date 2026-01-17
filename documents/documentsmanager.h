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
        return qobject_cast<DocumentTab*>(m_tabWidget->widget(m_documentIndex));
    };
    int documentIndexOf(DocumentTab *document) const {
        return m_tabWidget->indexOf(document);
    }

    QString currentDocumentName() const override;
    bool currentDocumentModified() const override;
    bool hasOpenDocuments() const override;
    bool hasAnythingSelected() const override;
    int documentCount() const override;

public slots:
    void createNewDocument();
    void openDocument(const QString &path);
    void saveCurrentDocument();
    void saveCurrentDocumentAs(const QString &path);
    void saveDocument(DocumentTab *document);
    void saveDocumentAs(DocumentTab *document, const QString &path);
    void saveAllDocuments();
    void closeDocument();
    void closeDocument(int index);
    void renameDocument(int index);
    void changeCurrentDocument(int index);
    void onTabMoved(int to);

signals:
    void documentCreated(DocumentTab *document);
    void documentOpened(DocumentTab *document);
    void documentClosed(DocumentTab *document);
    void documentChanged(int index);
    void documentModificationChanged(bool modified);

private:
    QTabWidget *m_tabWidget;
    QList<DocumentTab*> m_documents;
    QHash<DocumentTab*, QLabel*> m_labels;
    int m_documentIndex;

    int initializeNewDocument(DocumentTab &document);
    void drawDocumentBar(DocumentTab *document);
    void setupTabBarContextMenu();
    void showTabContextMenu(const QPoint &position);
};

#endif // DOCUMENTSMANAGER_H
