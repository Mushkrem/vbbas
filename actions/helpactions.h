#ifndef HELPACTIONS_H
#define HELPACTIONS_H

#include <QToolBar>
#include <QObject>
#include <QAction>
#include <QMenu>

class HelpActions : public QObject
{
    Q_OBJECT
public:
    explicit HelpActions(QWidget *parentWindow, QObject *parent = nullptr);

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

#endif // HELPACTIONS_H
