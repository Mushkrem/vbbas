#include "mainwindow.h"
#include "stylingutils.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    MainWindow window;
    window.show();

    if(Styling::applyStyling(&window) != EXIT_SUCCESS) {
        qDebug() << "Couldn't apply styling";
        return EXIT_FAILURE;
    }

    return a.exec();
}
