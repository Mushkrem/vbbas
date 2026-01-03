#include "docksmanager.h"

#include "tasksdock.h"

DocksManager::DocksManager(const QList<QDockWidget*> &docks, QObject *parent)
    : QObject(parent), m_docks(docks) {
    for (auto &dock : m_docks) {
        qDebug() << dock->objectName();

        if (auto *tasks = qobject_cast<TasksDock*>(dock)) {
            connect(tasks, &TasksDock::taskStarted,
                    this, &DocksManager::onTaskStarted);
        }
    }
}

void DocksManager::onTaskStarted(const QString &task) {
    qDebug() << "Task started: " << task;
}
