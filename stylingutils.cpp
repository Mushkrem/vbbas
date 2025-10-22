#include "stylingutils.h"
#include <QDir>
#include <qmenubar.h>

namespace Styling {

int applyQssIfAvailable(QWidget *widget, QString basePath) {
    QString key = widget->objectName();
    qDebug() << widget->palette();
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

    if(err != 0) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

}
