#include "documentsmanager.h"
#include "fileservice.h"

#include <QMenu>
#include <QStyle>
#include <QLabel>
#include <QTabBar>
#include <QFileInfo>
#include <QPushButton>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QAbstractButton>
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
    return doc ? doc->newTitle() : QString();
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
    return doc ? doc->isAnythingSelected() : false;
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

    if (!document->filePath().isEmpty())
        wrapper->setToolTip(document->filePath());

    auto *layout = new QHBoxLayout(wrapper);
    layout->setContentsMargins(6, 0, 3, 0);
    layout->setSpacing(6);
    layout->setAlignment(Qt::AlignLeft);

    QIcon icon = QIcon::fromTheme("document-new");
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
    }

    layout->addWidget(iconLabel);
    layout->addWidget(label);

    m_tabWidget->tabBar()->setTabButton(index, QTabBar::LeftSide, wrapper);
    m_labels.insert(document, label);
}

void DocumentsManager::createNewDocument() {
    auto *document = new DocumentTab;
    initializeNewDocument(document);

    emit documentCreated(document);
}

void DocumentsManager::openDocument(const QString &path) {
    // Check if document is already opened
    for (DocumentTab *document : std::as_const(m_documents)) {
        if (document->filePath() == path) {
            int index = documentIndexOf(document);
            changeCurrentDocument(index);
            return;
        }
    }
    auto *document = new DocumentTab;
    QString error;
    if (!FileService::loadDocument(document, path, error)) {
        QMessageBox messageBox(m_tabWidget);
        messageBox.setIconPixmap(messageBox.style()->standardIcon(QStyle::SP_MessageBoxCritical).pixmap(32, 32));
        messageBox.setWindowTitle(tr("Open Error"));
        messageBox.setText(tr("Failed to open document:\n"
                              "'%1'").arg(error));
        messageBox.exec();
        delete document;
        return;
    }

    document->setFilePath(path);
    initializeNewDocument(document);

    emit documentCreated(document);
}

int DocumentsManager::initializeNewDocument(DocumentTab *document) {
    m_documents.append(document);
    int index = m_tabWidget->addTab(document, "");
    changeCurrentDocument(index);

    connect(document, &DocumentTab::modifiedChanged,
            this, &DocumentsManager::documentModificationChanged);

    connect(document, &DocumentTab::selectionChanged,
            this, &DocumentsManager::documentSelectionChanged);

    connect(document, &DocumentTab::clipboardChanged,
            this, &DocumentsManager::clipboardChanged);

    document->initialize();

    drawDocumentBar(document);

    return index;
}

void DocumentsManager::saveDocument(DocumentTab *document) {
    if (!document) return;
    if (document->filePath().isEmpty()) {
        emit saveAsRequested(document);
        return;
    }

    QFileInfo currentFileInfo(document->filePath());
    QString currentFileName = currentFileInfo.fileName();
    QString newTitle = document->newTitle();

    if (!newTitle.endsWith("." + FileService::fileExtension()))
        newTitle += "." + FileService::fileExtension();

    if (currentFileName != newTitle) {
        // Title changed - need to rename the file
        QString newFilePath = QDir(currentFileInfo.absolutePath()).filePath(newTitle);

        if (!QFile::rename(document->filePath(), newFilePath)) {
            QMessageBox messageBox(m_tabWidget);
            messageBox.setIconPixmap(messageBox.style()->standardIcon(QStyle::SP_MessageBoxCritical).pixmap(32, 32));
            messageBox.setWindowTitle(tr("Rename Failed"));
            messageBox.setText(tr("Failed to rename file."
                                  "\nSaving with original name."));
            messageBox.exec();
            return;
        } else {
            document->setFilePath(newFilePath);
        }
    }

    QString error;
    if (!FileService::saveDocument(document, document->filePath(), error)) {
        qDebug() << "Error while saving document: " << error;
    }

    document->save();
    drawDocumentBar(document);
}
void DocumentsManager::saveCurrentDocument() {
    DocumentTab *document = currentDocument();
    saveDocument(document);
}

void DocumentsManager::saveDocumentAs(DocumentTab *document, const QString &filePath) {
    if (!document || filePath.isEmpty())
        return;

    document->setFilePath(filePath);
    saveDocument(document);
}

void DocumentsManager::saveCurrentDocumentAs(const QString &filePath) {
    DocumentTab *document = currentDocument();
    saveDocumentAs(document, filePath);
}

void DocumentsManager::saveAllDocuments() {
    QList<DocumentTab*> documentsNeedingSaveAs;
    for(DocumentTab *document : std::as_const(m_documents)) {
        if (!document->isModified())
            continue;

        if (document->filePath().isEmpty()) {
            documentsNeedingSaveAs.append(document);
        } else {
            saveDocument(document);
        }
    }

    if (!documentsNeedingSaveAs.isEmpty()) {
        QStringList names;
        for (auto *doc : documentsNeedingSaveAs) {
            names << doc->newTitle();
        }

        QMessageBox messageBox(m_tabWidget);
        messageBox.setIconPixmap(messageBox.style()->standardIcon(QStyle::SP_MessageBoxInformation).pixmap(32, 32));
        messageBox.setWindowTitle(tr("Save All"));
        messageBox.setText(tr("The following documents need file paths:\n%1\n"
                              "\nPlease save them individually using Save As.")
                               .arg(names.join("\n")));
        messageBox.exec();
    }
}

void DocumentsManager::closeDocument(int index) {
    DocumentTab *document = qobject_cast<DocumentTab*>(m_tabWidget->widget(index));
    if(!document)
        return;

    if (document->isModified()) {
        QMessageBox messageBox(m_tabWidget);
        messageBox.setIconPixmap(messageBox.style()->standardIcon(QStyle::SP_MessageBoxWarning).pixmap(32, 32));
        messageBox.setWindowTitle(tr("Unsaved Changes"));
        messageBox.setText(tr("The document \"%1\" has been modified. \nDo you want to save your changes?")
                               .arg(document->newTitle()));

        QPushButton *saveButton = messageBox.addButton(tr("Save"), QMessageBox::AcceptRole);
        QPushButton *discardButton = messageBox.addButton(tr("Don't save"), QMessageBox::DestructiveRole);
        QPushButton *cancelButton = messageBox.addButton(tr("Cancel"), QMessageBox::RejectRole);
        Q_UNUSED(discardButton);

        messageBox.exec();

        QAbstractButton *clicked = messageBox.clickedButton();
        if (clicked == saveButton) {
            saveDocument(document);
        } else if (clicked == cancelButton) {
            return;
        }
    }

    m_documents.removeOne(document);
    m_tabWidget->removeTab(index);
    document->deleteLater();
    if(index > 0)
        changeCurrentDocument(index - 1);

    emit documentClosed(document);
}

void DocumentsManager::renameDocument(int index) {
    DocumentTab *document = qobject_cast<DocumentTab*>(m_tabWidget->widget(index));
    if (!document)
        return;

    QString currentName = document->newTitle();
    QString extensionWithDot = "." + FileService::fileExtension();
    if (currentName.endsWith(FileService::fileExtension(), Qt::CaseInsensitive))
        currentName = currentName.left(currentName.length() - extensionWithDot.length());
    bool changed = false;

    QString newName = QInputDialog::getText(
        m_tabWidget,
        tr("Rename Document"),
        tr("Enter new name:"),
        QLineEdit::Normal,
        currentName,
        &changed
        );

    // .VIB DO NAZWY+

    newName.replace(" ", ""); // Remove whitespaces

    if (!changed || newName.isEmpty() || newName == currentName) {
        return;
    }

    if (!validNameRegex->match(newName).hasMatch()) {
        QStringList illegalChars;
        auto it = illegalCharsRegex->globalMatch(newName);
        while (it.hasNext()) {
            illegalChars << it.next().captured(0);
        }
        illegalChars.removeDuplicates();
        QMessageBox messageBox(m_tabWidget);
        messageBox.setIconPixmap(messageBox.style()->standardIcon(QStyle::SP_MessageBoxCritical).pixmap(32, 32));
        messageBox.setWindowTitle(tr("Invalid Name"));
        messageBox.setText(tr("Your input contains invalid characters. \n"
                              "File names cannot contain the following characters: "
                              "'%1'").arg(illegalChars.join(" ")));
        messageBox.exec();
        return;
    }

    if (!document->filePath().isEmpty()) {
        QFileInfo fileInfo(document->filePath());
        QString newFilePath = QDir(fileInfo.absolutePath()).filePath(newName);
        QFile file(newFilePath);
        if (file.exists()) {
            QMessageBox messageBox(m_tabWidget);
            messageBox.setIconPixmap(messageBox.style()->standardIcon(QStyle::SP_MessageBoxCritical).pixmap(32, 32));
            messageBox.setWindowTitle(tr("Invalid Name"));
            messageBox.setText(tr("File by the name you have specified already exists.\n"
                                  "Please choose a different name."));
            messageBox.exec();
            return;
        }
    }

    document->setTitle(newName);
    drawDocumentBar(document);
}

void DocumentsManager::closeDocument() {
    int index = m_tabWidget->currentIndex();
    closeDocument(index);
}

void DocumentsManager::onTabMoved(int to) {
    m_documentIndex = to;
}
