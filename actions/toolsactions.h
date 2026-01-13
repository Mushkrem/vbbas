#ifndef TOOLSACTIONS_H
#define TOOLSACTIONS_H

#include <QToolBar>
#include <QObject>
#include <QAction>
#include <QMenu>

class ToolsActions : public QObject
{
    Q_OBJECT
public:
    explicit ToolsActions(QWidget *parentWindow, QObject *parent = nullptr);

    QAction* dummyAction;

    void populateMenu(QMenu *menu);
    QToolBar* createToolBar(QWidget *parent);

signals:
    void dummyRequested();

private slots:
    void onDummyTriggered();

private:
    QWidget *m_parentWindow;
};

#endif // TOOLSACTIONS_H
