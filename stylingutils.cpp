#include "stylingutils.h"

#include <QGraphicsDropShadowEffect>
#include <QGuiApplication>
#include <QStyleFactory>
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
        palette.setColor(QPalette::Mid, QColor(35, 35, 35));
        palette.setColor(QPalette::Dark, QColor(28, 28, 28));
        palette.setColor(QPalette::Base, QColor(31, 31, 31));
        palette.setColor(QPalette::Window, QColor(30, 30, 30));
    } else if(scheme == Qt::ColorScheme::Light || scheme == Qt::ColorScheme::Unknown) {
        palette.setColor(QPalette::Midlight, QColor(240, 240, 240));
        palette.setColor(QPalette::Mid, QColor(216, 216, 216));
        palette.setColor(QPalette::Dark, QColor(209, 209, 209));
        palette.setColor(QPalette::Accent, QColor(196, 196, 196));
        palette.setColor(QPalette::Base, QColor(245, 245, 245));
        palette.setColor(QPalette::Window, QColor(239, 239, 239));
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
        qDebug() << "Looking for: " << key;
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

int applyStyling(QWidget *window) {
    const QString qssBasePath = ":/resources/qss";
    return applyQssIfAvailable(window, qssBasePath);;
}

void applyDropShadowEffect(QWidget* widget) {
    QGraphicsDropShadowEffect* dropShadowEffect = new QGraphicsDropShadowEffect(widget);
    dropShadowEffect->setColor(QColor(0, 0, 0, 150));
    dropShadowEffect->setBlurRadius(8.0);
    dropShadowEffect->setOffset(5.0, 5.0);

    widget->setGraphicsEffect(dropShadowEffect);
}

}
