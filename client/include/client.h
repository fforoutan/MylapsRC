#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <memory>

/**
 * @brief The Client class handles communication with the server.
 */
class Client : public QObject {
    Q_OBJECT

public:
    explicit Client(QObject* parent = nullptr);
    ~Client();

private:
    void loadSettings();
    QString ip_;
    int port_;
    std::unique_ptr<QTcpSocket> socket_;


};

#endif  // CLIENT_H
