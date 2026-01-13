#include "toolsactions.h"

ToolsActions::ToolsActions(QWidget *parentWindow, QObject *parent)
    : QObject{parent}, m_parentWindow(parentWindow)
{
    // *** Tools ***
    // Dummy
    dummyAction = new QAction(tr("&Dummy"), this);
    dummyAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::AudioCard));
    dummyAction->setShortcut(QKeySequence::fromString("Shift+Insert"));
    connect(dummyAction, &QAction::triggered, this, &ToolsActions::onDummyTriggered);
}

void ToolsActions::populateMenu(QMenu *menu) {
    menu->addAction(dummyAction);
}

QToolBar* ToolsActions::createToolBar(QWidget *parent) {
    QToolBar *toolbar = new QToolBar(tr("Tools Toolbar"), parent);
    toolbar->setObjectName("toolstoolbar");
    toolbar->setIconSize(QSize(16, 16));
    toolbar->setFloatable(false);
    toolbar->setMovable(true);

    toolbar->addAction(dummyAction);

    return toolbar;
}

void ToolsActions::onDummyTriggered() {
    emit dummyRequested();
}
