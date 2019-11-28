#include <QAtomicInteger>

#include "DataClient.h"
#include "LoggerProvider.h"

DataClient::DataClient(QObject *parent)
    : QObject(parent),
      loggerName(makeLoggerName()),
      tcpSocket(new QTcpSocket(this))
{
}

DataClient::~DataClient()
{
    LOGGER(this->loggerName)->log("Disconnected");
}

void
DataClient::connect(QString host, quint16 port)
{
    QObject::connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
        this, SLOT(error(QAbstractSocket::SocketError)));
    QObject::connect(tcpSocket, SIGNAL(connected()), this, SLOT(connected()));
    QObject::connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    LOGGER(this->loggerName)->log(QString("Trying to connect to %1:%2...").arg(host, QString::number(port)));
    tcpSocket->connectToHost(host, port);
}

void
DataClient::error(QAbstractSocket::SocketError socketError)
{
    LOGGER(this->loggerName)->log(this->tcpSocket->errorString());
}

void
DataClient::connected()
{
    LOGGER(this->loggerName)->log("Connected.");
}

void
DataClient::readyRead()
{
    QString newData = this->tcpSocket->readAll();
    LOGGER(this->loggerName)->log("New data: " + newData);
}

QString
DataClient::makeLoggerName() const
{
    static QAtomicInteger<int> index = 0;
    return QString("client%1").arg(++index);
}

void
DataClient::write(const QString& data)
{
    QTextStream out(this->tcpSocket);
    out << data;
}
