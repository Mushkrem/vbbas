#ifndef FILEACTIONS_H
#define FILEACTIONS_H

#include "../documents/idocumentinfo.h"

#include <QToolBar>
#include <QObject>
#include <QAction>
#include <QMenu>

class IDocumentInfo;
class DocumentTab;

class FileActions : public QObject
{
    Q_OBJECT
public:
    explicit FileActions(QWidget *parentWindow, QObject *parent = nullptr);

    void setDocumentInfo(IDocumentInfo *documentInfo);

    void populateMenu(QMenu *menu);
    QToolBar* createToolBar(QWidget *parent);
    void updateActionStates();

    QAction* newFileAction;
    QAction* openFileAction;
    QAction* closeFileAction;
    QAction* saveFileAction;
    QAction* saveAsFileAction;
    QAction* saveAllFilesAction;
    QAction* printFileAction;
    QAction* recentFilesAction;
    QAction* exitAction;

signals:
    void newFileRequested();
    void openFileRequested(const QString &filePath);
    void closeFileRequested();
    void saveFileRequested();
    void saveAsFileRequested(DocumentTab *document, const QString &filePath);
    void saveCurrentAsFileRequested(const QString &filepath);
    void saveAllFilesRequested();
    void printFileRequested();
    void recentFilesRequested();
    void exitRequested();

public slots:
    void onSaveAsFileTriggered(DocumentTab *document);

private slots:
    void onNewFileTriggered();
    void onOpenFileTriggered();
    void onCloseFileTriggered();
    void onSaveFileTriggered();
    void onCurrentSaveAsFileTriggered();
    void onSaveAllFilesTriggered();
    void onPrintFileTriggered();
    void onRecentFilesTriggered();
    void onExitTriggered();

private:
    QWidget *m_parentWindow;
    IDocumentInfo *m_documentInfo = nullptr;
};

#endif // FILEACTIONS_H
