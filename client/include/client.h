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
    /**
     * @brief Opens a file dialog to select a CSV file and sends its contents to the server.
     */
    Q_INVOKABLE void sendCSVData();

private slots:
    /**
     * @brief Handles messages received from the server and emits a signal with the response.
     */
    void readMessage();

    /**
     * @brief Handles socket error events.
     * @param socket_error The type of socket error.
     */
    void handleError(QAbstractSocket::SocketError socket_error);

private:
    /**
     * @brief Reads the contents of a CSV file and sends it to the server.
     * @param filename The name of the CSV file to read.
     */
    void readCSVAndSend(const QString& filename);

    /**
     * @brief Loads the settings from the configuration file.
     */
    void loadSettings();

    QString ip_;
    int port_;
    std::unique_ptr<QTcpSocket> socket_;

signals:
    void responseReceived(const QString& response);
};

#endif  // CLIENT_H
