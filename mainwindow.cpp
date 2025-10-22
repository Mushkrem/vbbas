#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "stylingutils.h"

#include <QGraphicsDropShadowEffect>
#include <QStyleHints>
#include <QEvent>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QList<QAction *> actions = ui->menubar->actions();
    for (QAction *action : actions) {
        QMenu *menu = action->menu();
        QGraphicsDropShadowEffect* dropShadowEffect = new QGraphicsDropShadowEffect(menu);
        dropShadowEffect->setColor(QColor(0, 0, 0, 150));
        dropShadowEffect->setBlurRadius(8.0);
        dropShadowEffect->setOffset(5.0, 5.0);

        menu->setGraphicsEffect(dropShadowEffect);

        if(action->text() == QString("View")) {
            QList<QAction *> viewActions = menu->actions();
            for(QAction *subAction : viewActions) {
                if(subAction->objectName() == "actionShowLeftSidebar") {
                    subAction->setIcon(QIcon::fromTheme("left_pane"));
                }
                if(subAction->objectName() == "actionShowRightSidebar") {
                    subAction->setIcon(QIcon::fromTheme("right_pane"));
                }
                qDebug() << subAction->icon().name();
            }
        }

    }
}

void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::ThemeChange) {
        auto scheme = QGuiApplication::styleHints()->colorScheme();
        qApp->setPalette(Styling::createCustomPalette(scheme));
        Styling::applyStyling(this);
    }

    QMainWindow::changeEvent(event); // Always call base implementation
}

MainWindow::~MainWindow()
{
    delete ui;
}
