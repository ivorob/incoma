#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSharedPointer<LoggerController> serverLogController(new LoggerController("Server log", "server.log"));
    QSharedPointer<LoggerController> client1LogController(new LoggerController("Client1 log", "client1.log"));
    QSharedPointer<LoggerController> client2LogController(new LoggerController("Client2 log", "client2.log"));

    loggerControllers.append(serverLogController);
    loggerControllers.append(client1LogController);
    loggerControllers.append(client2LogController);

    for (auto loggerController : loggerControllers) {
        ui->tabWidget->addTab(loggerController->getWidget(), loggerController->getTitle());
    }

    serverLogController->getLogger().log("Server test log 1");
    serverLogController->getLogger().log("Server test log 2");
    serverLogController->getLogger().log("Server test log 3");

    client1LogController->getLogger().log("Client1 test log 1");

    client2LogController->getLogger().log("Client2 test log 3");
    client2LogController->getLogger().log("Client2 test log 2");
    client2LogController->getLogger().log("Client2 test log 1");

}
