#pragma once

#include <QReadWriteLock>
#include <QHash>
#include <QString>

#include "Logger.h"

class LoggerProvider : public QObject {
    Q_OBJECT
private:
    LoggerProvider() = default;
public:
    static LoggerProvider& instance();
public:
    LoggerHolder get(const QString& name);
signals:
    void newLogger(QString name, LoggerHolder holder);
private:
    LoggerHolder addLogger(const QString& name);
private:
    QHash<QString, LoggerHolder> loggers;
    QReadWriteLock guard;
};

#define LOGGER(name) LoggerProvider::instance().get(name)
