#include <QApplication>

#include "MainWindow.h"
#include "LoggerProvider.h"
#include "LogFileDumper.h"

int
main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;

    LOGGER("server")->log("test1");
    LOGGER("server")->log("test2");
    LOGGER("server")->log("test3");

    LOGGER("client1")->log("test1");

    LOGGER("client2")->log("test3");
    LOGGER("client2")->log("test2");
    LOGGER("client2")->log("test1");

    window.show();
    return app.exec();
}
