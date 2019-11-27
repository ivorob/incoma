#pragma once

#include "Logger.h"
#include "LoggerWidget.h"
#include "LogFileDumper.h"

class LoggerController {
public:
    LoggerController(QString title, QString filename);

    Logger& getLogger();
    QWidget *getWidget();
    QString getTitle() const;
private:
    LoggerWidget *loggerWidget;
    LogFileDumper logFileDumper;
    Logger logger;
};
