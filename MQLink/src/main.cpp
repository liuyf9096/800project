#ifdef INCLUDE_GUI
#include <QApplication>
#include "mainwindow.h"
#else
#include <QCoreApplication>
#include "csignal"
#endif

#include "f_init.h"

#ifndef INCLUDE_GUI
void signalHandler(int /*signal*/) {
    QCoreApplication::quit();
}

void cleanup() {}
#endif

int main(int argc, char *argv[])
{
#ifdef INCLUDE_GUI
    QApplication app(argc, argv);
#else
    QCoreApplication app(argc, argv);
#endif

    servers_init();

#ifdef INCLUDE_GUI
    MainWindow w;
    w.show();
#else
    /* Process before quit */
    std::signal(SIGINT, signalHandler);     // ctrl+c
    std::signal(SIGTERM, signalHandler);    // terminate
    QObject::connect(&app, &QCoreApplication::aboutToQuit, &cleanup);
#endif

    return app.exec();
}
