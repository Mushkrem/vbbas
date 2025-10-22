#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QGraphicsDropShadowEffect>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QList<QAction *> actions = ui->menubar->actions();
    for(int i = 0; i < actions.size(); i++) {
        QAction *action = actions.at(i);
        QMenu *menu = action->menu();
        QGraphicsDropShadowEffect* dropShadowEffect = new QGraphicsDropShadowEffect(menu);
        dropShadowEffect->setColor(QColor(0, 0, 0, 150));
        dropShadowEffect->setBlurRadius(8.0);
        dropShadowEffect->setOffset(5.0, 5.0);

        menu->setGraphicsEffect(dropShadowEffect);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
