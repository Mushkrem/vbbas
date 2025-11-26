#include "mainwindow.h"
#include "stylingutils.h"

#include <QApplication>
#include <QStyleFactory>
#include <QStyleHints>

#include <QFile>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    auto scheme = QGuiApplication::styleHints()->colorScheme();
    app.setPalette(Styling::createCustomPalette(scheme));

    MainWindow window;

    if(Styling::applyStyling(&window) != EXIT_SUCCESS) {
        qDebug() << "Couldn't apply styling";
        return EXIT_FAILURE;
    }

    window.show();

    return app.exec();
}
