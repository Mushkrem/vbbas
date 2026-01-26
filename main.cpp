#include "mainwindow.h"
#include "stylingutils.h"

#include <QStyleHints>
#include <QApplication>
#include <QStyleFactory>
#include <QFontDatabase>

#include <QFile>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_Use96Dpi);

    QApplication app(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QApplication::setAttribute(Qt::AA_DontShowIconsInMenus, false);

    int fontCode = QFontDatabase::addApplicationFont(":/resources/fonts/CascadiaCode.ttf");
    int fontMono = QFontDatabase::addApplicationFont(":/resources/fonts/CascadiaMono.ttf");
    if (fontCode < 0 || fontMono < 0) {
        qWarning() << "Failed to load Cascadia fonts";
    }

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
