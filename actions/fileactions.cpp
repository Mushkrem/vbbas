#include "fileactions.h"

#include <QFileDialog>
#include <QMessageBox>

FileActions::FileActions(QWidget *parentWindow, QObject *parent)
    : QObject{parent}, m_parentWindow(parentWindow)
{
    // *** Files ***
    // New
    newFileAction = new QAction(tr("&New..."), this);
        newFileAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::DocumentNew));
        newFileAction->setShortcut(QKeySequence::fromString("Ctrl+N"));
        connect(newFileAction, &QAction::triggered, this, &FileActions::onNewFileTriggered);

    // Open
    openFileAction = new QAction(tr("&Open..."), this);
        openFileAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::FolderOpen));
        openFileAction->setShortcut(QKeySequence::fromString("Ctrl+O"));
        connect(openFileAction, &QAction::triggered, this, &FileActions::onOpenFileTriggered);

    // Close
    closeFileAction = new QAction(tr("&Close"), this);
        closeFileAction->setShortcut(QKeySequence::fromString("Ctrl+F4"));
        connect(closeFileAction, &QAction::triggered, this, &FileActions::onCloseFileTriggered);

    // Save
    saveFileAction = new QAction(tr("&Save"), this);
        saveFileAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSave));
        saveFileAction->setShortcut(QKeySequence::fromString("Ctrl+S"));
        connect(saveFileAction, &QAction::triggered, this, &FileActions::onSaveFileTriggered);

    // Save As
    saveAsFileAction = new QAction(tr("&Save As..."), this);
        saveAsFileAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSaveAs));
        connect(saveAsFileAction, &QAction::triggered, this, &FileActions::onSaveAsFileTriggered);

    // Save All
    saveAllFilesAction = new QAction(tr("&Save All"), this);
        saveAllFilesAction->setShortcut(QKeySequence::fromString("Ctrl+Shift+S"));
        connect(saveAllFilesAction, &QAction::triggered, this, &FileActions::onSaveAllFilesTriggered);

    // Print
    printFileAction = new QAction(tr("&Print..."), this);
        printFileAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::DocumentPrint));
        printFileAction->setShortcut(QKeySequence::fromString("Ctrl+P"));
        connect(printFileAction, &QAction::triggered, this, &FileActions::onPrintFileTriggered);

    // Recent
    recentFilesAction = new QAction(tr("&Recent Files..."), this);
        recentFilesAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackStart));
        connect(recentFilesAction, &QAction::hovered, this, &FileActions::onRecentFilesTriggered);

    // Exit
    exitAction = new QAction(tr("&Exit"), this);
        exitAction->setShortcut(QKeySequence::fromString("Alt+F4"));
        connect(exitAction, &QAction::hovered, this, &FileActions::onExitTriggered);
}

void FileActions::onNewFileTriggered() {
    emit newFileRequested();
}
void FileActions::onOpenFileTriggered() {
    QString fileName = QFileDialog::getOpenFileName(m_parentWindow, tr("Open File"));
    if (!fileName.isEmpty())
        emit openFileRequested(fileName);
}
void FileActions::onCloseFileTriggered() {
    emit closeFileRequested();
}

void FileActions::onSaveFileTriggered() {
    emit saveFileRequested();
}
void FileActions::onSaveAsFileTriggered() {
    emit saveAsFileRequested();
}
void FileActions::onSaveAllFilesTriggered() {
    emit saveAllFilesRequested();
}

void FileActions::onPrintFileTriggered() {
    emit printFileRequested();
}

void FileActions::onRecentFilesTriggered() {
    emit recentFilesRequested();
}

void FileActions::onExitTriggered() {
    emit exitRequested();
}
