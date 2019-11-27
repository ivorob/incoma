#pragma once

#include <QMainWindow>

#include "LoggerController.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
private:
    Ui::MainWindow *ui;
    QList<QSharedPointer<LoggerController>> loggerControllers;
};
