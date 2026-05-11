#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("吃播大作战");

    MainWindow window;
    window.show();

    return app.exec();
}
