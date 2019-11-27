#pragma once

#include <QMainWindow>

#include "LoggerController.h"
#include "LoggerProvider.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void newLogger(QString name, LoggerHolder holder);
private:
    Ui::MainWindow *ui;
    QList<QSharedPointer<LoggerController>> loggerControllers;
};
