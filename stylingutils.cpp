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

    // Change pallete colors depending on the scheme.
    if(scheme == Qt::ColorScheme::Dark) {
        palette.setColor(QPalette::Midlight, QColor(39, 39, 39));
    } else if(scheme == Qt::ColorScheme::Light || scheme == Qt::ColorScheme::Unknown) {
        palette.setColor(QPalette::Midlight, QColor(255, 255, 255));
    }

    return palette;
}

int applyQssIfAvailable(QWidget *widget, QString basePath) {
    QString key = widget->objectName();
    QString qssFilePath = QString("%1/%2.css").arg(basePath, key);
    int result = EXIT_SUCCESS;

    // Look for related .css files to apply the styling.
    if(!key.isEmpty()) {
        QFile file(qssFilePath);
        if(file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            widget->setStyleSheet(file.readAll());
            file.close();
        }
    }

    // Recursively try applying styling to its children.
    for(QObject *child : widget->children()) {
        if(auto child_widget = qobject_cast<QWidget *>(child)) {
            int child_err = applyQssIfAvailable(child_widget, basePath);
            if(child_err == EXIT_FAILURE)
                result = EXIT_FAILURE;
        }
    }

    return result;
}

int applyStyling(MainWindow *window) {
    const QString qssBasePath = ":/resources/qss";
    int err = 0;

    // Loop through window's children and try applying styling.
    for(QObject * child : window->children()) {
        if(auto child_widget = qobject_cast<QWidget *>(child)) {
            err += applyQssIfAvailable(child_widget, qssBasePath);
        }
    }

    if(err != 0) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

}
