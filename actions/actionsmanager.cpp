#include "actionsmanager.h"

ActionsManager::ActionsManager(QWidget *parentWindow, QObject *parent)
    : QObject(parent) {
    file = new FileActions(parentWindow, this);
    edit = new EditActions(parentWindow, this);
    view = new ViewActions(parentWindow, this);
    build = new BuildActions(parentWindow, this);
    tools = new ToolsActions(parentWindow, this);
    help = new HelpActions(parentWindow, this);

    object = new ObjectActions(parentWindow, this);
}

// MainWindow top-menu
void ActionsManager::setupMenus(QMenu *fileMenu, QMenu *editMenu, QMenu *viewMenu, QMenu *buildMenu, QMenu *toolsMenu, QMenu *helpMenu) {
    file->populateMenu(fileMenu);
    edit->populateMenu(editMenu);
    view->populateMenu(viewMenu);
    build->populateMenu(buildMenu);
    tools->populateMenu(toolsMenu);
    help->populateMenu(helpMenu);
}

// MainWindow Toolbar
QList<QToolBar*> ActionsManager::createToolBars(QWidget *parent) {
    QList<QToolBar*> toolbars;
    toolbars.append(file->createToolBar(parent));
    toolbars.append(edit->createToolBar(parent));
    toolbars.append(build->createToolBar(parent));
    toolbars.append(help->createToolBar(parent));

    return toolbars;
}
