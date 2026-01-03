#ifndef ACTIONSMANAGER_H
#define ACTIONSMANAGER_H

#include "fileactions.h"
#include "editactions.h"

#include <QObject>

class ActionsManager : public QObject {
    Q_OBJECT
public:
    explicit ActionsManager(QWidget *parentWindow, QObject *parent = nullptr);

    FileActions *file;
    EditActions *edit;

    void setupMenus(QMenu *fileMenu, QMenu *editMenu);
    QList<QToolBar*> createToolBars(QWidget *parent);
};

#endif // ACTIONSMANAGER_H
