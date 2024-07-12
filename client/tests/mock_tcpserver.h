#ifndef MOCK_TCPSERVER_H
#define MOCK_TCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QByteArray>

class MockTcpServer : public QTcpServer {
    Q_OBJECT

public:
    MockTcpServer(QObject *parent = nullptr) : QTcpServer(parent), clientSocket(nullptr) {}

    void startServer(quint16 port) {
        if (!this->listen(QHostAddress::Any, port)) {
            qDebug() << "Server could not start!";
        } else {
            qDebug() << "Server started!";
        }
    }

    QTcpSocket* getClientSocket() {
        return clientSocket;
    }

protected:
    void incomingConnection(qintptr socketDescriptor) override {
        clientSocket = new QTcpSocket(this);
        clientSocket->setSocketDescriptor(socketDescriptor);
        connect(clientSocket, &QTcpSocket::readyRead, this, &MockTcpServer::handleReadyRead);
        emit newClientConnected();
    }

signals:
    void newClientConnected();

private slots:
    void handleReadyRead() {
        QByteArray data = clientSocket->readAll();
        qDebug() << "Server received:" << data;
        // Echo the data back to the client
        clientSocket->write(data);
    }

private:
    QTcpSocket *clientSocket;
};

#endif // MOCK_TCPSERVER_H
