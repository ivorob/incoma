#include "LoggerController.h"

LoggerController::LoggerController(QString title, QString filename)
    : loggerWidget(new LoggerWidget(title)),
      logFileDumper(filename)
{
    logger.addView(loggerWidget->getView());
    logger.addView(&logFileDumper);
}

Logger&
LoggerController::getLogger()
{
    return this->logger;
}

QWidget *
LoggerController::getWidget()
{
    return this->loggerWidget;
}

QString
LoggerController::getTitle() const
{
    return this->loggerWidget->windowTitle();
}
