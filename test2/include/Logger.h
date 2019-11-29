#pragma once

#include <QString>
#include <QStringListModel>
#include <QSharedPointer>
#include <QAbstractItemView>

class Logger {
public:
    Logger();

    void log(QString logString);
    void addView(QAbstractItemView *view);
private:
    QSharedPointer<QStringListModel> model;
};

typedef QSharedPointer<Logger> LoggerHolder;
