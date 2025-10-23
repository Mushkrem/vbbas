#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QToolBar *fileToolBar;

protected:
    void changeEvent(QEvent *event) override;

private:
    Ui::MainWindow *ui;
    void addToolBars();
};
#endif // MAINWINDOW_H
