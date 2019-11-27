#include "LoggerProvider.h"

LoggerProvider&
LoggerProvider::instance()
{
    static LoggerProvider loggerProvider;
    return loggerProvider;
}

LoggerHolder
LoggerProvider::get(const QString& name)
{
    LoggerHolder result;

    this->guard.lockForRead();

    auto it = this->loggers.find(name);
    if (it == this->loggers.end()) {
        result.reset(new Logger);

        this->guard.unlock();
        result = addLogger(name);
    } else {
        result = it.value();
    }

    return result;
}

LoggerHolder
LoggerProvider::addLogger(const QString& name)
{
    QWriteLocker locker(&this->guard);

    auto it = this->loggers.find(name);
    if (it == this->loggers.end()) {
        LoggerHolder logger(new Logger);
        this->loggers.insert(name, logger);
        return logger;
    }

    return it.value();
}
