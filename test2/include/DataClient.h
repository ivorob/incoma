#pragma once

#include <QTcpSocket>

class DataClient : public QObject {
    Q_OBJECT
public:
    DataClient(QObject *parent = nullptr);
    ~DataClient();

    void connect(QString host, quint16 port);
    void write(const QString& data);
private slots:
    void error(QAbstractSocket::SocketError socketError);
    void connected();
    void readyRead();
private:
    QString makeLoggerName() const;
private:
    QString loggerName;
    QTcpSocket *tcpSocket;
};
