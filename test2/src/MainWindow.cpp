#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    LoggerProvider& loggerProvider = LoggerProvider::instance();
    QObject::connect(&loggerProvider, SIGNAL(newLogger(QString, LoggerHolder)),
            this, SLOT(newLogger(QString, LoggerHolder)));
}

MainWindow::~MainWindow()
{
    LoggerProvider& loggerProvider = LoggerProvider::instance();
    QObject::disconnect(&loggerProvider, SIGNAL(newLogger(QString, LoggerHolder)),
            this, SLOT(newLogger(QString, LoggerHolder)));
}

void
MainWindow::newLogger(QString name, LoggerHolder holder)
{
    try {
        QSharedPointer<LoggerController> loggerController(new LoggerController(this));
        loggerController->attach(name, holder);
        this->loggerControllers.append(loggerController);

        ui->tabWidget->addTab(loggerController->getWidget(), loggerController->getTitle());
    } catch (const std::exception& e) {
    }
}
