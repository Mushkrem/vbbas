#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "actions/fileactions.h"
#include "documents/documentsmanager.h"
#include "docks/docksmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void extracted(QList<QAction *> &actions);
    ~MainWindow();

    QToolBar *fileToolBar;

protected:
    void changeEvent(QEvent *event) override;

private:
    Ui::MainWindow *ui;
    QTabWidget *central;

    FileActions *fileActions;
    DocumentsManager *documentsManager;
    DocksManager *docksManager;

    void connectActions();

    void onOpenFile();
    void addToolBars();
    void addActions();
};
#endif // MAINWINDOW_H
