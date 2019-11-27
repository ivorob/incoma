#include "LoggerWidget.h"
#include "ui_LoggerWidget.h"

LoggerWidget::LoggerWidget(QString title, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::LoggerWidget)
{
    ui->setupUi(this);
    setWindowTitle(title);
}

QAbstractItemView *
LoggerWidget::getView() const
{
    return ui->listView;
}
