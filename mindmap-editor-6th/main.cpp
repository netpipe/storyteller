#include "mainwindow.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("SimpleMind");

    MainWindow w;
    w.show();

    return a.exec();
}
