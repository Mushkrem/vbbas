#ifndef DOCKSMANAGER_H
#define DOCKSMANAGER_H

#include <QDockWidget>

class DocksManager : public QObject {
    Q_OBJECT
public:
    explicit DocksManager(const QList<QDockWidget*> &docks, QObject *parent = nullptr);
private:
    QList<QDockWidget*> m_docks;

    void onTaskStarted(const QString &task);
};

#endif // DOCKSMANAGER_H
