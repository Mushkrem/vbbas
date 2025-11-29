#ifndef STYLINGUTILS_H
#define STYLINGUTILS_H

#include "mainwindow.h"
#include <QWidget>
#include <QString>

namespace Styling {
QPalette createCustomPalette(Qt::ColorScheme scheme);
int applyQssIfAvailaible(QWidget *widget, const QString basePath);
int applyStyling(MainWindow *window);
void applyDropShadowEffect(QWidget* widget);
}

#endif // STYLINGUTILS_H
