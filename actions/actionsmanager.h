#ifndef ACTIONSMANAGER_H
#define ACTIONSMANAGER_H

#include "fileactions.h"
#include "editactions.h"
#include "viewactions.h"
#include "buildactions.h"
#include "toolsactions.h"
#include "helpactions.h"
#include "objectactions.h"

#include <QObject>

class ActionsManager : public QObject {
    Q_OBJECT
public:
    explicit ActionsManager(QWidget *parentWindow, QObject *parent = nullptr);

    FileActions *file;
    EditActions *edit;
    ViewActions *view;
    BuildActions *build;
    ToolsActions *tools;
    HelpActions *help;

    ObjectActions *object;

    void setupMenus(QMenu *fileMenu, QMenu *editMenu, QMenu *viewMenu, QMenu *buildMenu, QMenu *toolsMenu, QMenu *helpMenu);
    QList<QToolBar*> createToolBars(QWidget *parent);
};

#endif // ACTIONSMANAGER_H
