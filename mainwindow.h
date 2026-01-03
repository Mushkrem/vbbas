#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "documents/documentsmanager.h"
#include "actions/actionsmanager.h"
#include "docks/docksmanager.h"

#include <QMainWindow>

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

    ActionsManager *actionsManager;
    DocumentsManager *documentsManager;
    DocksManager *docksManager;

    void connectActions();
    void setupUI();
};
#endif // MAINWINDOW_H
