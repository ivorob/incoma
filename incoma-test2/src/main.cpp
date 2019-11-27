#include <QApplication>
#include <QWidget>
#include <QStringListModel>
#include <QListView>
#include <QVBoxLayout>
#include <QDateTime>

#include "Logger.h"
#include "LogFileDumper.h"
#include "LoggerWidget.h"

int
main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    LoggerWidget loggerWidget("Server log");

    Logger testLogger;
    LogFileDumper logFileDumper("1.log");
    testLogger.addView(&logFileDumper);

    testLogger.log("Test string 1");
    testLogger.addView(loggerWidget.getView());
    testLogger.log("Test string 2");

    try {
        LogFileDumper newLogDumper("/invalid_log_path.log");
        testLogger.addView(&newLogDumper);
    } catch (const std::exception& e) {
        testLogger.log(e.what());
    }

    testLogger.log("Test string 3");

    loggerWidget.show();
    return app.exec();
}
