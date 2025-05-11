#include "mainwindow.h"
#include "f_init.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    servers_init();

    MainWindow w;
    w.show();
    return a.exec();
}
