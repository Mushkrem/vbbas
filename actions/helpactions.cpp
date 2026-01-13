#include "helpactions.h"

HelpActions::HelpActions(QWidget *parentWindow, QObject *parent)
    : QObject{parent}, m_parentWindow(parentWindow)
{
    // *** Help ***
    // Dummy
    dummyAction = new QAction(tr("&Help "), this);
    dummyAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::HelpFaq));
    connect(dummyAction, &QAction::triggered, this, &HelpActions::onDummyTriggered);
}

void HelpActions::populateMenu(QMenu *menu) {
    menu->addAction(dummyAction);
}

QToolBar* HelpActions::createToolBar(QWidget *parent) {
    QToolBar *toolbar = new QToolBar(tr("Help Toolbar"), parent);
    toolbar->setObjectName("helptoolbar");
    toolbar->setIconSize(QSize(16, 16));
    toolbar->setFloatable(false);
    toolbar->setMovable(true);

    toolbar->addAction(dummyAction);

    return toolbar;
}

void HelpActions::onDummyTriggered() {
    emit dummyRequested();
}
