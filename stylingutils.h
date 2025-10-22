#ifndef STYLINGUTILS_H
#define STYLINGUTILS_H

#include "mainwindow.h"
#include <QWidget>
#include <QString>

namespace Styling {
int applyQssIfAvailaible(QWidget *widget, const QString basePath);
int applyStyling(MainWindow *window);
}

#endif // STYLINGUTILS_H
