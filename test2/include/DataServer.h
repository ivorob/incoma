#pragma once

#include <QTcpServer>
#include <QTcpSocket>
#include <QAtomicInteger>

class DataServer : public QObject {
    Q_OBJECT
public:
    DataServer(quint16 port, quint16 maxClients, QObject *parent = nullptr);
private slots:
    void newConnection();
    void readClientData();
    void removeClient();
private:
    void sendToClient(QTcpSocket *tcpSocket, const QString& data) const;

    int newClient();
    bool isLimitOfClientsReached(int count) const;
private:
    const QString loggerName = "server";
private:
    QTcpServer *tcpServer;
    quint16 maxClients;
    QAtomicInteger<int> clientsCount;
};
