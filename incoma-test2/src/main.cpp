#include <QApplication>

#include "MainWindow.h"
#include "LoggerProvider.h"
#include "LogFileDumper.h"

int
main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    LogFileDumper logFileDumper("new.log");
    LOGGER("new")->addView(&logFileDumper);
    LOGGER("new")->log("test1");
    LOGGER("new")->log("test2");

    MainWindow window;
    window.show();
    return app.exec();
}
