#ifndef EDITACTIONS_H
#define EDITACTIONS_H

#include "../documents/idocumentinfo.h"

#include <QToolBar>
#include <QObject>
#include <QAction>
#include <QMenu>

class EditActions : public QObject
{
    Q_OBJECT
public:
    explicit EditActions(QWidget *parentWindow, QObject *parent = nullptr);

    void setDocumentInfo(IDocumentInfo *documentInfo);

    QAction* undoAction;
    QAction* redoAction;
    QAction* cutAction;
    QAction* copyAction;
    QAction* pasteAction;
    QAction* deleteAction;
    QAction* selectAllAction;
    QAction* findAction;
    QAction* replaceAction;
    QAction* propertiesAction;

    void populateMenu(QMenu *menu);
    QToolBar* createToolBar(QWidget *parent);
    void updateActionStates();

signals:
    void undoRequested();
    void redoRequested();
    void cutRequested();
    void copyRequested();
    void pasteRequested();
    void deleteRequested();
    void selectAllRequested();
    void findRequested();
    void replaceRequested();
    void propertiesRequested();

private slots:
    void onUndoTriggered();
    void onRedoTriggered();
    void onCutTriggered();
    void onCopyTriggered();
    void onPasteTriggered();
    void onDeleteTriggered();
    void onSelectAllTriggered();
    void onFindTriggered();
    void onReplaceTriggered();
    void onPropertiesTriggered();

private:
    QWidget *m_parentWindow;
    IDocumentInfo *m_documentInfo = nullptr;
};

#endif // EDITACTIONS_H
