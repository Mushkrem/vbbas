#ifndef EDITACTIONS_H
#define EDITACTIONS_H

#include <QToolBar>
#include <QObject>
#include <QAction>
#include <QMenu>

class EditActions : public QObject
{
    Q_OBJECT
public:
    explicit EditActions(QWidget *parentWindow, QObject *parent = nullptr);

    QAction* UndoAction;
    QAction* RedoAction;
    QAction* CutAction;
    QAction* CopyAction;
    QAction* PasteAction;
    QAction* DeleteAction;
    QAction* SelectAllAction;
    QAction* FindAction;
    QAction* ReplaceAction;
    QAction* PropertiesAction;

    void populateMenu(QMenu *menu);
    QToolBar* createToolBar(QWidget *parent);

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
};

#endif // EDITACTIONS_H
