#ifndef VIEWACTIONS_H
#define VIEWACTIONS_H

#include <QToolBar>
#include <QObject>
#include <QAction>
#include <QMenu>

class ViewActions : public QObject
{
    Q_OBJECT
public:
    explicit ViewActions(QWidget *parentWindow, QObject *parent = nullptr);

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

#endif // VIEWACTIONS_H
