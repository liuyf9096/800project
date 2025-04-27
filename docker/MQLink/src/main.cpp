#include <QCoreApplication>
#include "csignal"
#include "f_init.h"

void signalHandler(int /*signal*/) {
    QCoreApplication::quit();
}

void cleanup() {}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    servers_init();

    /* Process before quit */
    std::signal(SIGINT, signalHandler);     // ctrl+c
    std::signal(SIGTERM, signalHandler);    // terminate
    QObject::connect(&app, &QCoreApplication::aboutToQuit, &cleanup);

    return app.exec();
}
