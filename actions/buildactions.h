#ifndef BUILDACTIONS_H
#define BUILDACTIONS_H

#include "../documents/idocumentinfo.h"

#include <QToolBar>
#include <QObject>
#include <QAction>
#include <QMenu>

class BuildActions : public QObject
{
    Q_OBJECT
public:
    explicit BuildActions(QWidget *parentWindow, QObject *parent = nullptr);

    void setDocumentInfo(IDocumentInfo *documentInfo);

    QAction* simulateAction;
    QAction* stopAction;

    void populateMenu(QMenu *menu);
    QToolBar* createToolBar(QWidget *parent);
    void updateActionStates();

signals:
    void simulationRequested();
    void stopRequested();

private slots:
    void onSimulationTriggered();
    void onStopTriggered();

private:
    QWidget *m_parentWindow;
    IDocumentInfo *m_documentInfo = nullptr;
};

#endif // BUILDACTIONS_H
