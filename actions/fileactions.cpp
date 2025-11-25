#include "fileactions.h"

#include <QFileDialog>
#include <QMessageBox>

FileActions::FileActions(QWidget *parentWindow, QObject *parent)
    : QObject{parent}, m_parentWindow(parentWindow)
{
    newFileAction = new QAction(tr("&New..."), this);
        newFileAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::DocumentNew));
        newFileAction->setShortcut(QKeySequence::fromString("Ctrl+N"));
    openFileAction = new QAction(tr("&Open..."), this);
        openFileAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::FolderOpen));
        openFileAction->setShortcut(QKeySequence::fromString("Ctrl+O"));
    closeFileAction = new QAction(tr("&Close"), this);
        closeFileAction->setShortcut(QKeySequence::fromString("Ctrl+F4"));

    saveFileAction = new QAction(tr("&Save"), this);
        saveFileAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSave));
        saveFileAction->setShortcut(QKeySequence::fromString("Ctrl+S"));
    saveAsFileAction = new QAction(tr("&Save As..."), this);
        saveAsFileAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSaveAs));
    saveAllFilesAction = new QAction(tr("&Save All"), this);
        saveAllFilesAction->setShortcut(QKeySequence::fromString("Ctrl+Shift+S"));

    printFileAction = new QAction(tr("&Print..."), this);
        printFileAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::DocumentPrint));
        printFileAction->setShortcut(QKeySequence::fromString("Ctrl+P"));

    recentFilesAction = new QAction(tr("&Recent Files..."), this);
        recentFilesAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackStart));

    exitAction = new QAction(tr("&Exit"), this);

    connect(newFileAction, &QAction::triggered, this, &FileActions::newFileRequested);
    connect(openFileAction, &QAction::triggered, this, &FileActions::openFileRequested);
    connect(closeFileAction, &QAction::triggered, this, &FileActions::closeFileRequested);

    connect(saveFileAction, &QAction::triggered, this, &FileActions::saveFileRequested);
    connect(saveAsFileAction, &QAction::triggered, this, &FileActions::saveAsFileRequested);
    connect(saveAllFilesAction, &QAction::triggered, this, &FileActions::saveAllFilesRequested);

    connect(printFileAction, &QAction::triggered, this, &FileActions::printFileRequested);
    connect(recentFilesAction, &QAction::hovered, this, &FileActions::recentFilesRequested);

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
