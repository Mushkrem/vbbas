#include "stylingutils.h"
#include <QStyleFactory>
#include <QGuiApplication>
#include <QStyleHints>
#include <qmenubar.h>
#include <QToolBar>
#include <QStyle>
#include <QDir>


namespace Styling {

QPalette createCustomPalette(Qt::ColorScheme scheme) {
    QPalette palette = QStyleFactory::create("Fusion")->standardPalette();

    if(scheme == Qt::ColorScheme::Dark) {
        palette.setColor(QPalette::Midlight, QColor(39, 39, 39));
    } else {
        palette.setColor(QPalette::Midlight, QColor(255,255, 255));
    }

    return palette;
}

int applyQssIfAvailable(QWidget *widget, QString basePath) {
    QString key = widget->objectName();
    const auto scheme = QGuiApplication::styleHints()->colorScheme();
    // qDebug() << style->colorScheme();
    // qDebug() << widget->dynamicPropertyNames();
    if(key.isEmpty())
        return EXIT_FAILURE;

    QString qssFilePath = QString("%1/%2.css").arg(basePath, key);

    QDir qssDir(basePath);
    if(qssDir.exists(key + ".css")) {
        QFile file(qssFilePath);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            widget->setStyleSheet(file.readAll());
            file.close();
        }
    }

    for(QObject *child : widget->children()) {
        if(auto child_widget = qobject_cast<QWidget *>(child)) {
            applyQssIfAvailable(child_widget, basePath);
        }
    }

    return EXIT_SUCCESS;
}

int applyStyling(MainWindow *window) {
    const QString qssBasePath = ":/resources/qss";
    int err = 0;

    err += applyQssIfAvailable(window->menuBar(), qssBasePath);
    err += applyQssIfAvailable(window->centralWidget(), qssBasePath);

    QList<QToolBar *> toolbars = window->findChildren<QToolBar *>();
    for(QToolBar *toolbar : toolbars) {
        err += applyQssIfAvailable(toolbar, qssBasePath);
    }

    if(err != 0) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

}
