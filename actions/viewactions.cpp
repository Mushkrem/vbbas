#include "viewactions.h"

ViewActions::ViewActions(QWidget *parentWindow, QObject *parent)
    : QObject{parent}, m_parentWindow(parentWindow)
{
    // *** Help ***
    // Dummy
    dummyAction = new QAction(tr("&Dummy"), this);
    dummyAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::AudioCard));
    dummyAction->setShortcut(QKeySequence::fromString("Shift+Insert"));
    connect(dummyAction, &QAction::triggered, this, &ViewActions::onDummyTriggered);
}

void ViewActions::populateMenu(QMenu *menu) {
    menu->addAction(dummyAction);
}

QToolBar* ViewActions::createToolBar(QWidget *parent) {
    QToolBar *toolbar = new QToolBar(tr("View Toolbar"), parent);
    toolbar->setObjectName("viewtoolbar");
    toolbar->setIconSize(QSize(16, 16));
    toolbar->setFloatable(false);
    toolbar->setMovable(true);

    toolbar->addAction(dummyAction);

    return toolbar;
}

void ViewActions::onDummyTriggered() {
    emit dummyRequested();
}
