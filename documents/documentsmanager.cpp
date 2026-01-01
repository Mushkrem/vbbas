#include "documentsmanager.h"

#include <QDebug>

#include <QRandomGenerator>

DocumentsManager::DocumentsManager(QTabWidget *tabWidget, QObject *parent)
    : QObject(parent), m_tabWidget(tabWidget) {}

void DocumentsManager::createNewDocument() {
    auto *document = new DocumentTab;
    m_documents.append(document);

    int index = m_tabWidget->addTab(document, "Untitled");
    m_tabWidget->setCurrentIndex(index);

    connect(document, &DocumentTab::modifiedChanged,
            this, [this, document](bool modified) {
                int index = m_tabWidget->indexOf(document);
                if (index >= 0) {
                    QString title = m_tabWidget->tabText(index);
                    if (modified && !title.endsWith("*"))
                        title += "*";
                    else if (!modified)
                        title.remove("*");
                    m_tabWidget->setTabText(index, title);
                }
            });

    emit documentCreated(document);
}

void DocumentsManager::openDocument(const QString &path) {
    auto *document = new DocumentTab;
    // to do

    emit documentOpened(document);
}

void DocumentsManager::saveDocument(DocumentTab *document) {
    // to do
}

void DocumentsManager::closeDocument(int index) {
    DocumentTab *document = qobject_cast<DocumentTab*>(m_tabWidget->widget(index));
    if(!document)
        return;

    m_documents.removeOne(document);
    m_tabWidget->removeTab(index);
    document->deleteLater();

    emit documentClosed(document);
}

void DocumentsManager::closeDocument() {
    int index = m_tabWidget->currentIndex();
    DocumentsManager::closeDocument(index);
}
