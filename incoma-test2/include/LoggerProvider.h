#pragma once

#include <QReadWriteLock>
#include <QHash>
#include <QString>

#include "Logger.h"

typedef QSharedPointer<Logger> LoggerHolder;

class LoggerProvider {
    LoggerProvider() = default;
public:
    static LoggerProvider& instance();
public:
    LoggerHolder get(const QString& name);
private:
    LoggerHolder addLogger(const QString& name);
private:
    QHash<QString, LoggerHolder> loggers;
    QReadWriteLock guard;
};

#define LOGGER(name) LoggerProvider::instance().get(name)
