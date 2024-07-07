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

    /**
     * @brief Sends a request to the server to get the average time for a specific driver.
     * @param driver_number The driver number to request the average time for.
     */
    Q_INVOKABLE void requestAverageTime(const QString& driver_number);


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
     * @brief Converts a time in seconds to a formatted string.
     * @param seconds The time in seconds to format.
     * @return The formatted time string in HH:MM:SS:MSMS format.
     */
    QString formatTime(double seconds);

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
