#include <QTcpSocket>
#include <QTextStream>

#include "DataServer.h"
#include "LoggerProvider.h"

DataServer::DataServer(quint16 port, quint16 maxClients, QObject *parent)
    : QObject(parent),
      tcpServer(new QTcpServer(this)),
      maxClients(maxClients),
      clientsCount(0)
{
    if (!tcpServer->listen(QHostAddress::Any, port)) {
        throw std::runtime_error(QString("Cannot listen port %1").arg(port).toStdString());
    }

    QObject::connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));

    LOGGER(this->loggerName)->log(QString("Server is started on port %1").arg(port));
    LOGGER(this->loggerName)->log(QString("Waiting connection..."));
}

void
DataServer::newConnection()
{
    QTcpSocket *tcpSocket = tcpServer->nextPendingConnection();
    if (tcpSocket != nullptr) {
        QString clientAddress = tcpSocket->peerAddress().toString();
        QString clientPort = QString::number(tcpSocket->peerPort());

        if (isLimitOfClientsReached(newClient())) {
            LOGGER(this->loggerName)->log(
                    QString("New connection from address %1 port %2").arg(clientAddress, clientPort));

            QObject::connect(tcpSocket, SIGNAL(disconnected()),
                    tcpSocket, SLOT(removeClient()));
            QObject::connect(tcpSocket, SIGNAL(disconnected()),
                    tcpSocket, SLOT(deleteLater()));

            QObject::connect(tcpSocket, SIGNAL(readyRead()),
                    this, SLOT(readClientData()));

            sendToClient(tcpSocket, "OK\n");
        } else {
            LOGGER(this->loggerName)->log(
                    QString("New connection from address %1 port %2 - REJECTED").arg(clientAddress, clientPort));

            sendToClient(tcpSocket, "BUSY\n");
            tcpSocket->close();

            removeClient();

        }
    }
}

void
DataServer::readClientData()
{
    QTcpSocket *tcpSocket = qobject_cast<QTcpSocket *>(sender());
    while (!tcpSocket->atEnd()) {
        QTextStream input(tcpSocket);
        QString newData = input.readLine();

        LOGGER(this->loggerName)->log(QString("New server data: %1").arg(newData));
    }
}

void
DataServer::sendToClient(QTcpSocket *tcpSocket, const QString& data) const
{
    QTextStream out(tcpSocket);
    out << data;
}

int
DataServer::newClient()
{
    return ++this->clientsCount;
}

void
DataServer::removeClient()
{
    --this->clientsCount;
}

bool
DataServer::isLimitOfClientsReached(int count) const
{
    return count <= this->maxClients;
}
