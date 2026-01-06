#include "documentsmanager.h"

#include <QLabel>
#include <QTabBar>
#include <QFileInfo>
#include <QHBoxLayout>

DocumentsManager::DocumentsManager(QTabWidget *tabWidget, QObject *parent)
    : QObject(parent), m_tabWidget(tabWidget) {}

// IDocument
QString DocumentsManager::currentDocumentName() const {
    DocumentTab *doc = currentDocument();
    return doc ? doc->title() : QString();
}

bool DocumentsManager::currentDocumentModified() const {
    DocumentTab *doc = currentDocument();
    return doc ? doc->isModified() : false;
}

bool DocumentsManager::hasOpenDocuments() const {
    return !m_documents.empty();
}

int DocumentsManager::documentCount() const {
    return m_documents.size();
}

//

void DocumentsManager::createNewDocument() {
    auto *document = new DocumentTab;
    m_documents.append(document);

    int index = m_tabWidget->addTab(document, "");
    m_tabWidget->setCurrentIndex(index);

    auto *wrapper = new QWidget;
    wrapper->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    wrapper->setAttribute(Qt::WA_TransparentForMouseEvents);

    auto *layout = new QHBoxLayout(wrapper);
    layout->setContentsMargins(6, 0, 6, 0);
    layout->setSpacing(0);

    QIcon icon = QIcon::fromTheme("format-justify-left");

    auto *iconLabel = new QLabel;
    iconLabel->setPixmap(icon.pixmap(16, 16));
    iconLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    auto *label = new QLabel(document->title() + "     .");
    label->setTextFormat(Qt::RichText);
    label->setStyleSheet("margin-left: 6px;");
    label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    label->setAttribute(Qt::WA_TransparentForMouseEvents);

    layout->addWidget(iconLabel);
    layout->addWidget(label);

    m_tabWidget->tabBar()->setTabButton(index, QTabBar::LeftSide, wrapper);

    m_labels.insert(document, label);

    connect(document, &DocumentTab::modifiedChanged,
            this, [this, document](bool modified) {
                QLabel *label = m_labels.value(document);
                if (!label)
                    return;

                QString base = document->title();
                if (modified)
                    label->setText("<i>" + base + "*</i>");
                else
                    label->setText(base);
                m_tabWidget->tabBar()->update();
                m_tabWidget->updateGeometry();
                emit documentModificationChanged(modified);
            });

    document->initialize();

    emit documentCreated(document);
}

void DocumentsManager::openDocument(const QString &path) {
    auto *document = new DocumentTab;
    document->setTitle(QFileInfo(path).fileName());
    // to do

    emit documentOpened(document);
}

void DocumentsManager::saveDocument(DocumentTab *document) {
    // to do
    qDebug("save!");
    document->save();
}

void DocumentsManager::saveCurrentDocument() {
    DocumentTab *document = DocumentsManager::currentDocument();
    if(!document)
        return;

    DocumentsManager::saveDocument(document);
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
