#ifndef TASKSDOCK_H
#define TASKSDOCK_H

#include "dockbase.h"

#include <QElapsedTimer>
#include <QMap>

class QTableWidget;
class QPushButton;
class QLabel;
class QTimer;
class QProgressBar;

class TasksDock : public DockBase
{
    Q_OBJECT

public:
    explicit TasksDock(QWidget *parent = nullptr);

    enum class TaskStatus {
        Pending,
        Queued,
        Running,
        Completed,
        Failed,
        Cancelled
    };
    Q_ENUM(TaskStatus)

signals:
    void taskStarted(const QString &taskName);
    void taskFinished(const QString &taskName, bool success, qint64 elapsedMs);
    void allTasksCompleted(bool allSucceeded);
    void compilationRequested();

// public slots:
//     void runAllTasks();
//     void runSingleTask(int row);
//     void stopAllTasks();
//     void resetAllTasks();

//     void onExternalTaskCompleted(const QString &taskName, bool success);

protected:
    QWidget* createContent() override;

private:
    struct TaskInfo {
        QString name;
        QString description;
        TaskStatus status = TaskStatus::Pending;
        qint64 elapsedMs = 0;
    };

    QList<TaskInfo> m_tasks;
};

#endif // TASKSDOCK_H
