#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "actions/fileactions.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void extracted(QList<QAction *> &actions);
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QToolBar *fileToolBar;

protected:
    void changeEvent(QEvent *event) override;

private:
    Ui::MainWindow *ui;
    FileActions *fileActions;
    void connectActions();

    void onOpenFile();
    void addToolBars();
    void addActions();
};
#endif // MAINWINDOW_H
