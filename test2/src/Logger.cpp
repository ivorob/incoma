#include <QDateTime>

#include "Logger.h"

Logger::Logger()
    : model(new QStringListModel)
{
}

void
Logger::log(QString logString)
{
    if (model->insertRow(model->rowCount())) {
        QModelIndex index = model->index(model->rowCount() - 1, 0);
        model->setData(index, QString("%1 %2").arg(
                    QDateTime::currentDateTime().toString(), logString));
    }
}

void
Logger::addView(QAbstractItemView *view)
{
    if (view) {
        view->setModel(model.get());
    }
}
