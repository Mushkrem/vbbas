#include "documentsmanager.h"
#include "fileservice.h"

#include <QMenu>
#include <QLabel>
#include <QTabBar>
#include <QFileInfo>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QRegularExpression>

namespace {
    Q_GLOBAL_STATIC(QRegularExpression, validNameRegex, ("^[A-Za-z0-9_]+$"))
    Q_GLOBAL_STATIC(QRegularExpression, illegalCharsRegex, ("[^A-Za-z0-9_]"))
}

DocumentsManager::DocumentsManager(QTabWidget *tabWidget, QObject *parent)
    : QObject(parent), m_tabWidget(tabWidget)
{
    setupTabBarContextMenu();
}

void DocumentsManager::changeCurrentDocument(int index) {
    m_documentIndex = index;
    m_tabWidget->setCurrentIndex(index);
    emit documentChanged(index);
}

// IDocument
QString DocumentsManager::currentDocumentName() const {
    DocumentTab *doc = currentDocument();
    return doc ? doc->title() : QString();
}

void DocumentsManager::setupTabBarContextMenu() {
    m_tabWidget->tabBar()->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(m_tabWidget->tabBar(), &QTabBar::customContextMenuRequested,
            this, &DocumentsManager::showTabContextMenu);
}

bool DocumentsManager::currentDocumentModified() const {
    DocumentTab *doc = currentDocument();
    return doc ? doc->isModified() : false;
}

bool DocumentsManager::hasOpenDocuments() const {
    return !m_documents.empty();
}

bool DocumentsManager::hasAnythingSelected() const {
    DocumentTab *doc = currentDocument();
    return doc ? doc->isModified() : false;
}

int DocumentsManager::documentCount() const {
    return m_documents.size();
}

// ~IDocument

void DocumentsManager::showTabContextMenu(const QPoint &position) {
    int tabIndex = m_tabWidget->tabBar()->tabAt(position);
    if (tabIndex < 0)
        return;

    QMenu contextMenu(m_tabWidget);
    QAction *renameAction = contextMenu.addAction(tr("Rename..."));
    QAction *closeAction = contextMenu.addAction(tr("Close"));

    contextMenu.addSeparator();

    QAction *closeOthersAction = contextMenu.addAction(tr("Close Others"));
    QAction *closeAllAction = contextMenu.addAction(tr("Close All"));

    QAction *selectedAction = contextMenu.exec(m_tabWidget->tabBar()->mapToGlobal(position));
    if (!selectedAction)
        return;

    // Handle actions
    if (selectedAction == renameAction) {
        renameDocument(tabIndex);
    } else if (selectedAction == closeAction) {
        closeDocument(tabIndex);
    } else if (selectedAction == closeOthersAction) {
        for (int i = m_tabWidget->count() - 1; i >= 0; --i) {
            if (i != tabIndex) {
                closeDocument(i);
            }
        }
    } else if (selectedAction == closeAllAction) {
        for (int i = m_tabWidget->count() - 1; i >= 0; --i) {
            closeDocument(i);
        }
    }
}

void DocumentsManager::drawDocumentBar(DocumentTab *document) {
    int index = documentIndexOf(document);
    auto *wrapper = new QWidget;
    wrapper->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    wrapper->setAttribute(Qt::WA_TransparentForMouseEvents);

    auto *layout = new QHBoxLayout(wrapper);
    layout->setContentsMargins(6, 0, 3, 0);
    layout->setSpacing(6);
    layout->setAlignment(Qt::AlignLeft);

    QIcon icon = QIcon::fromTheme("format-justify-left");
    auto *iconLabel = new QLabel;
    iconLabel->setPixmap(icon.pixmap(16, 16));
    iconLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    QString title = document->newTitle();
    auto *label = new QLabel(title);
    label->setTextFormat(Qt::RichText);
    label->setAlignment(Qt::AlignLeft | Qt::AlignTop); // Unless I'm blind AlignTop looks better than AlignCenter
    label->setAttribute(Qt::WA_TransparentForMouseEvents);
    if (document->isModified()) {
        title.append("*");
        label->setText(title);
        // label->setStyleSheet("font-style: italic;");
    }

    layout->addWidget(iconLabel);
    layout->addWidget(label);

    m_tabWidget->tabBar()->setTabButton(index, QTabBar::LeftSide, wrapper);
    m_labels.insert(document, label);
}

void DocumentsManager::createNewDocument() {
    auto *document = new DocumentTab;
    m_documents.append(document);

    int index = m_tabWidget->addTab(document, "");
    changeCurrentDocument(index);
    drawDocumentBar(document);

    connect(document, &DocumentTab::modifiedChanged,
            this, &DocumentsManager::documentModificationChanged);

    document->initialize();

    emit documentCreated(document);
}

void DocumentsManager::renameDocument(int index) {
    DocumentTab *document = qobject_cast<DocumentTab*>(m_tabWidget->widget(index));
    if (!document)
        return;

    QString currentName = document->newTitle();
    bool changed = false;

    QString newName = QInputDialog::getText(
        m_tabWidget,
        tr("Rename Document"),
        tr("Enter new name:"),
        QLineEdit::Normal,
        currentName,
        &changed
    );

    newName.replace(" ", ""); // Remove whitespaces

    if (changed && !newName.isEmpty() && newName != currentName) {
        if (!validNameRegex->match(newName).hasMatch()) {
            QStringList illegalChars;
            auto it = illegalCharsRegex->globalMatch(newName);
            while (it.hasNext()) {
                illegalChars << it.next().captured(0);
            }
            illegalChars.removeDuplicates();

            QMessageBox::critical(
                m_tabWidget,
                tr("Invalid Name"),
                tr("Your input contains invalid characters.\n"
                   "File names cannot contain the following characters: "
                   "'%1'").arg(illegalChars.join(" "))
            );
            return;
        }
        document->setTitle(newName);
        drawDocumentBar(document);
    }
}

void DocumentsManager::openDocument(const QString &path) {
    auto *document = new DocumentTab;
    document->setTitle(QFileInfo(path).fileName());

    // to do
    // changeCurrentDocument(index);

    emit documentOpened(document);
}

void DocumentsManager::saveDocument(DocumentTab *document) {
    document->save();
    drawDocumentBar(document);
}

void DocumentsManager::saveDocumentAs(DocumentTab *document, const QString &path) {
    QString filePath = path;
    if (filePath.isNull()) {
        filePath = QFileDialog::getSaveFileUrl(
            m_tabWidget,
            tr("Save As..."),
            document->newTitle(),
            "Algorithm Files (*.vib)"
        ).toString();
        if (filePath.isNull())
            return;
    }

    document->setFilePath(filePath);
    saveDocument(document);
}

void DocumentsManager::saveCurrentDocument() {
    DocumentTab *document = currentDocument();
    if(!document)
        return;

    // If file changed name or the path is null saveAs.
    if(document->title() != document->newTitle() || document->filePath().isNull()) {
        saveDocumentAs(document, nullptr);
        return;
    }

    saveDocument(document);
}

void DocumentsManager::saveCurrentDocumentAs(const QString &path) {
    DocumentTab *document = currentDocument();
    if(!document)
        return;

    saveDocumentAs(document, path);
}

void DocumentsManager::saveAllDocuments() {
    for(DocumentTab *document : std::as_const(m_documents)) {
        if (!document->isModified())
            continue;

        const bool needsSaveAs =
            document->title() != document->newTitle() ||
            document->filePath().isNull();

        if (needsSaveAs) {
            saveDocumentAs(document, nullptr);
        } else {
            saveDocument(document);
        }
    }
}

void DocumentsManager::closeDocument(int index) {
    DocumentTab *document = qobject_cast<DocumentTab*>(m_tabWidget->widget(index));
    if(!document)
        return;

    m_documents.removeOne(document);
    m_tabWidget->removeTab(index);
    document->deleteLater();
    if(index > 0)
        changeCurrentDocument(index - 1);

    emit documentClosed(document);
}

void DocumentsManager::closeDocument() {
    int index = m_tabWidget->currentIndex();
    closeDocument(index);
}

void DocumentsManager::onTabMoved(int to) {
    m_documentIndex = to;
}
