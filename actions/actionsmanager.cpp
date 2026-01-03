#include "actionsmanager.h"

ActionsManager::ActionsManager(QWidget *parentWindow, QObject *parent)
    : QObject(parent) {
    file = new FileActions(parentWindow, this);
    edit = new EditActions(parentWindow, this);

    qDebug() << "thjis!";
}

void ActionsManager::setupMenus(QMenu *fileMenu, QMenu *editMenu) {
    file->populateMenu(fileMenu);
    edit->populateMenu(editMenu);
}

QList<QToolBar*> ActionsManager::createToolBars(QWidget *parent) {
    QList<QToolBar*> toolbars;
    toolbars.append(file->createToolBar(parent));
    toolbars.append(edit->createToolBar(parent));

    return toolbars;
}
