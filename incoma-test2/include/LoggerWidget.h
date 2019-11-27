#pragma once

#include <QWidget>
#include <QAbstractItemView>

namespace Ui {
class LoggerWidget;
}

class LoggerWidget : public QWidget {
    Q_OBJECT
public:
    LoggerWidget(QString title, QWidget *parent = nullptr);

    QAbstractItemView *getView() const;
private:
    Ui::LoggerWidget *ui;
};
