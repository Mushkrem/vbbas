#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDir>
#include <QGraphicsDropShadowEffect>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap pix(":/resources/img/f9c90a63363570d7e442e0926babc8b9.png");
    // int w = ui->label_pic->width();
    // int h = ui->label_pic->height();
    // ui->label_pic->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));

    QDir resourceDir(":/resources/img");
    qDebug() << "Files in resource: " << resourceDir.entryList().size();

    QList<QAction *> actions = ui->menubar->actions();
    for(int i = 0; i < actions.size(); i++) {
        QAction *action = actions.at(i);
        QMenu *menu = action->menu();
        QGraphicsDropShadowEffect* dropShadowEffect = new QGraphicsDropShadowEffect(menu);
        dropShadowEffect->setColor(QColor(0, 0, 0, 150));
        dropShadowEffect->setBlurRadius(8.0);
        dropShadowEffect->setOffset(5.0, 5.0);

        menu->setGraphicsEffect(dropShadowEffect);
        qDebug() << "Found: " << menu;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
