#ifndef FILEACTIONS_H
#define FILEACTIONS_H

#include <QObject>
#include <QAction>

class FileActions : public QObject
{
    Q_OBJECT
public:
    explicit FileActions(QWidget *parentWindow, QObject *parent = nullptr);

    QAction* newFileAction;
    QAction* openFileAction;
    QAction* closeFileAction;

    QAction* saveFileAction;
    QAction* saveAsFileAction;
    QAction* saveAllFilesAction;

    QAction* printFileAction;

    QAction* recentFilesAction;

    QAction* exitAction;

private slots:
    void newFileRequested();
    void openFileRequested();
    void closeFileRequested();

    void saveFileRequested();
    void saveAsFileRequested();
    void saveAllFilesRequested();

    void printFileRequested();

    void recentFilesRequested();

    void exitRequested();

private:
    QWidget *m_parentWindow;
};

#endif // FILEACTIONS_H
