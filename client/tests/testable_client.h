#ifndef TESTABLE_CLIENT_H
#define TESTABLE_CLIENT_H

#include "include/client.h"
#include "mock_qtcpsocket.h"

class TestableClient : public Client {
public:
    TestableClient(QObject *parent = nullptr, MockQTcpSocket *mockSocket = nullptr)
        : Client(parent), mockSocket_(mockSocket) {
        if (mockSocket_) {
            socket_.reset(mockSocket_);
            connect(mockSocket_, &QTcpSocket::readyRead, this, &Client::readMessage);
        }
    }

private:
    MockQTcpSocket *mockSocket_;
};

#endif // TESTABLE_CLIENT_H
