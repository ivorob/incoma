#include <QApplication>
#include <QThread>

#include "MainWindow.h"
#include "LoggerProvider.h"
#include "DataServer.h"
#include "DataClient.h"

int
main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    try {
        DataServer *server = new DataServer(9999, 4, &mainWindow);

        for (int i = 0; i < 8; ++i) {
            DataClient *client = new DataClient(server);
            client->connect("localhost", 9999);
            client->write(QString("Data from client%1").arg(i));
        }

        return app.exec();
    } catch (const std::exception& e) {
        LOGGER("error")->log(e.what());
    }

    return app.exec();
}
