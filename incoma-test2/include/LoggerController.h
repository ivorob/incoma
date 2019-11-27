#pragma once

#include "Logger.h"
#include "LoggerWidget.h"
#include "LogFileDumper.h"

class LoggerController {
public:
    LoggerController(QWidget *parent);

    void attach(QString name, LoggerHolder logger);

    QWidget *getWidget();
    QString getTitle() const;
private:
    LoggerWidget *lazyLoadWidget(const QString& name);
    LogFileDumper *lazyLoadLogFileDumper(const QString& name);
    QString makeLoggerWidgetTitle(const QString& name) const;
    QString makeLogFileName(const QString& name) const;
private:
    QWidget *parent;
    LoggerWidget *loggerWidget;
    QSharedPointer<LogFileDumper> logFileDumper;
    QMutex mutex;
};
