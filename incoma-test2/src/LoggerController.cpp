#include <QUuid>

#include "LoggerController.h"

LoggerController::LoggerController(QWidget *parent)
    : parent(parent),
      loggerWidget()
{
}

QWidget *
LoggerController::getWidget()
{
    return this->loggerWidget;
}

QString
LoggerController::getTitle() const
{
    return this->loggerWidget != nullptr ? this->loggerWidget->windowTitle() : QString();
}

void
LoggerController::attach(QString name, LoggerHolder logger)
{
    if (logger) {
        QMutexLocker locker(&this->mutex);

        logger->addView(lazyLoadWidget(name)->getView());
        logger->addView(lazyLoadLogFileDumper(name));
    }
}

LoggerWidget *
LoggerController::lazyLoadWidget(const QString& name)
{
    if (!this->loggerWidget) {
        this->loggerWidget = new LoggerWidget(makeLoggerWidgetTitle(name), parent);
    }

    return this->loggerWidget;
}

QString
LoggerController::makeLoggerWidgetTitle(const QString& name) const
{
    QString widgetTitle = name.toLower();
    if (!name.isEmpty()) {
        widgetTitle.replace(0, 1, widgetTitle[0].toUpper());
    } else {
        widgetTitle = QUuid().toString();
    }

    widgetTitle += " log";
    return widgetTitle;
}

LogFileDumper *
LoggerController::lazyLoadLogFileDumper(const QString& name)
{
    if (!this->logFileDumper) {
        this->logFileDumper.reset(new LogFileDumper(makeLogFileName(name)));
    }

    return this->logFileDumper.get();
}

QString
LoggerController::makeLogFileName(const QString& name) const
{
    return name.toLower() + ".log";
}
