#ifndef MOCK_QTCPSOCKET_H
#define MOCK_QTCPSOCKET_H

#include <QTcpSocket>
#include <gmock/gmock.h>

class MockQTcpSocket : public QTcpSocket {
    Q_OBJECT

public:
    MockQTcpSocket(QObject* parent = nullptr) : QTcpSocket(parent) {}

    MOCK_METHOD(QByteArray, readAll, (), ());
    MOCK_METHOD(QAbstractSocket::SocketState, state, (), ());
};

#endif // MOCK_QTCPSOCKET_H
