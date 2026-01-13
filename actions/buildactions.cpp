#include "buildactions.h"

BuildActions::BuildActions(QWidget *parentWindow, QObject *parent)
    : QObject{parent}, m_parentWindow(parentWindow)
{
    // *** Build ***
    // Simulate
    simulateAction = new QAction(tr("&Run Simulation"), this);
    simulateAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackStart));
    simulateAction->setShortcut(QKeySequence::fromString("Ctrl+R"));
    connect(simulateAction, &QAction::triggered, this, &BuildActions::onSimulationTriggered);

    // Stop
    stopAction = new QAction(tr("&Stop Simulation"), this);
    stopAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackStop));
    stopAction->setShortcut(QKeySequence::fromString("Shift+F5"));
    connect(stopAction, &QAction::triggered, this, &BuildActions::onStopTriggered);

}

void BuildActions::populateMenu(QMenu *menu) {
    menu->addAction(simulateAction);
    menu->addAction(stopAction);
}

QToolBar* BuildActions::createToolBar(QWidget *parent) {
    QToolBar *toolbar = new QToolBar(tr("Build Toolbar"), parent);
    toolbar->setObjectName("buildtoolbar");
    toolbar->setIconSize(QSize(16, 16));
    toolbar->setFloatable(false);
    toolbar->setMovable(true);

    toolbar->addAction(simulateAction);
    toolbar->addAction(stopAction);

    return toolbar;
}

void BuildActions::setDocumentInfo(IDocumentInfo *documentInfo) {
    m_documentInfo = documentInfo;
    updateActionStates();
}

void BuildActions::onSimulationTriggered() {
    emit simulationRequested();
}

void BuildActions::onStopTriggered() {
    emit stopRequested();
}

void BuildActions::updateActionStates()
{
    if (!m_documentInfo) return;

    bool hasOpenDocuments = m_documentInfo->hasOpenDocuments();

    simulateAction->setEnabled(hasOpenDocuments);
    stopAction->setEnabled(hasOpenDocuments);
}
