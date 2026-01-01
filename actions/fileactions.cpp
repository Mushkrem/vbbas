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
        connect(newFileAction, &QAction::triggered, this, &FileActions::newFileRequested);

    // Open
    openFileAction = new QAction(tr("&Open..."), this);
        openFileAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::FolderOpen));
        openFileAction->setShortcut(QKeySequence::fromString("Ctrl+O"));
        connect(openFileAction, &QAction::triggered, this, &FileActions::openFileRequested);

    // Close
    closeFileAction = new QAction(tr("&Close"), this);
        closeFileAction->setShortcut(QKeySequence::fromString("Ctrl+F4"));
        connect(closeFileAction, &QAction::triggered, this, &FileActions::closeFileRequested);

    // Save
    saveFileAction = new QAction(tr("&Save"), this);
        saveFileAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSave));
        saveFileAction->setShortcut(QKeySequence::fromString("Ctrl+S"));
        connect(saveFileAction, &QAction::triggered, this, &FileActions::saveFileRequested);

    // Save As
    saveAsFileAction = new QAction(tr("&Save As..."), this);
        saveAsFileAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSaveAs));
        connect(saveAsFileAction, &QAction::triggered, this, &FileActions::saveAsFileRequested);

    // Save All
    saveAllFilesAction = new QAction(tr("&Save All"), this);
        saveAllFilesAction->setShortcut(QKeySequence::fromString("Ctrl+Shift+S"));
        connect(saveAllFilesAction, &QAction::triggered, this, &FileActions::saveAllFilesRequested);

    // Print
    printFileAction = new QAction(tr("&Print..."), this);
        printFileAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::DocumentPrint));
        printFileAction->setShortcut(QKeySequence::fromString("Ctrl+P"));
        connect(printFileAction, &QAction::triggered, this, &FileActions::printFileRequested);

    // Recent
    recentFilesAction = new QAction(tr("&Recent Files..."), this);
        recentFilesAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackStart));
        connect(recentFilesAction, &QAction::hovered, this, &FileActions::recentFilesRequested);

    // *** Move these somewhere else (to do) ***
    exitAction = new QAction(tr("&Exit"), this);
        connect(exitAction, &QAction::hovered, this, &FileActions::exitRequested);
}

void FileActions::newFileRequested() {}
void FileActions::openFileRequested() {
    QString fileName = QFileDialog::getOpenFileName(m_parentWindow, tr("Open File"));
    if (!fileName.isEmpty()) {
        QMessageBox::information(m_parentWindow, tr("File Selected"), fileName);
    }
}
void FileActions::closeFileRequested() {}

void FileActions::saveFileRequested() {}
void FileActions::saveAsFileRequested() {}
void FileActions::saveAllFilesRequested() {}

void FileActions::printFileRequested() {}

void FileActions::recentFilesRequested() {}

void FileActions::exitRequested() {}
