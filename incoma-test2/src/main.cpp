#include <QApplication>
#include <QWidget>
#include <QStringListModel>
#include <QListView>
#include <QVBoxLayout>
#include <QDateTime>

#include "Logger.h"
#include "LogFileDumper.h"

int
main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QWidget widget;
    widget.setWindowTitle("Hello, World!");

    QVBoxLayout *layout = new QVBoxLayout;
    QListView *listView = new QListView;
    layout->addWidget(listView);
    widget.setLayout(layout);
    widget.show();

    Logger testLogger;
    LogFileDumper logFileDumper("1.log");
    testLogger.addView(&logFileDumper);

    testLogger.log("Test string 1");
    testLogger.addView(listView);
    testLogger.log("Test string 2");

    try {
        LogFileDumper newLogDumper("/invalid_log_path.log");
        testLogger.addView(&newLogDumper);
    } catch (const std::exception& e) {
        testLogger.log(e.what());
    }

    testLogger.log("Test string 3");

    return app.exec();
}
