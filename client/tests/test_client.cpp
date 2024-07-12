#define BOOST_TEST_MODULE test_client
#include <boost/test/unit_test.hpp>
#include "include/client.h"
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QtTest/QSignalSpy>
#include <QTemporaryFile>
#include <QDir>
#include <QTcpSocket>
#include <QTest>

class TestClient : public Client {
public:
    TestClient(QObject* parent = nullptr) : Client(parent) {}
    using Client::loadSettings;
    using Client::ip_;
    using Client::port_;
    using Client::readCSVAndSend;
    using Client::readMessage;
    std::unique_ptr<QTcpSocket>& getSocket() { return socket_; }
};

BOOST_AUTO_TEST_CASE(test_loadSettings) {
    // Prepare a configuration file
    QString configDir = QString(SOURCE_DIR) + "/resources/config/";
    QDir().mkpath(configDir);
    QFile configFile(configDir + "config.ini");
    if (configFile.open(QIODevice::WriteOnly)) {
        QTextStream out(&configFile);
        out << "[Network]\n";
        out << "IP=192.168.1.1\n";
        out << "Port=12345\n";
        configFile.close();
    }

    // Instantiate the client and load settings
    TestClient client;
    client.loadSettings();

    // Check if the settings are loaded correctly
    BOOST_CHECK_EQUAL(client.ip_.toStdString(), "192.168.1.1");
    BOOST_CHECK_EQUAL(client.port_, 12345);
}

BOOST_AUTO_TEST_CASE(test_loadSettings_invalidIP) {
    // Prepare a configuration file with an invalid IP
    QString configDir = QString(SOURCE_DIR) + "/resources/config/";
    QDir().mkpath(configDir);
    QFile configFile(configDir + "config.ini");
    if (configFile.open(QIODevice::WriteOnly)) {
        QTextStream out(&configFile);
        out << "[Network]\n";
        out << "IP=invalid_ip\n";
        out << "Port=12345\n";
        configFile.close();
    }

    // Instantiate the client and load settings
    TestClient client;
    client.loadSettings();

    // Check if the settings are loaded correctly (default IP should be used)
    BOOST_CHECK_EQUAL(client.ip_.toStdString(), "127.0.0.1");
    BOOST_CHECK_EQUAL(client.port_, 12345);
}

BOOST_AUTO_TEST_CASE(test_loadSettings_invalidPort) {
    // Prepare a configuration file with an invalid port
    QString configDir = QString(SOURCE_DIR) + "/resources/config/";
    QDir().mkpath(configDir);
    QFile configFile(configDir + "config.ini");
    if (configFile.open(QIODevice::WriteOnly)) {
        QTextStream out(&configFile);
        out << "[Network]\n";
        out << "IP=192.168.1.1\n";
        out << "Port=invalid_port\n";
        configFile.close();
    }

    // Instantiate the client and load settings
    TestClient client;
    client.loadSettings();

    // Check if the settings are loaded correctly (default port should be used)
    BOOST_CHECK_EQUAL(client.ip_.toStdString(), "192.168.1.1");
    BOOST_CHECK_EQUAL(client.port_, 8080);
}
BOOST_AUTO_TEST_CASE(test_formatTime) {
    TestClient client;

    QString result = client.formatTime(3661.123);
    BOOST_CHECK_EQUAL(result.toStdString(), "01:01:01:123");

    result = client.formatTime(59.999);
    BOOST_CHECK_EQUAL(result.toStdString(), "00:00:59:999");

    result = client.formatTime(3600);
    BOOST_CHECK_EQUAL(result.toStdString(), "01:00:00:000");
}
BOOST_AUTO_TEST_CASE(test_readCSVAndSend) {
    TestClient client;

    QTemporaryDir tempDir;
    QString tempFilePath = tempDir.path() + "/test.csv";
    QFile tempFile(tempFilePath);
    if (tempFile.open(QIODevice::WriteOnly)) {
        QTextStream out(&tempFile);
        out << "Driver,Time\n";
        out << "1,120.5\n";
        out << "2,150.0\n";
        tempFile.close();
    }

    QSignalSpy spy(&client, &Client::responseReceived);
    client.readCSVAndSend(tempFilePath);

    BOOST_CHECK_EQUAL(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QString response = arguments.at(0).toString();
    BOOST_CHECK(response.isEmpty());

    // To further validate, we might need to mock socket operations,
    // or inspect client internals if possible.
}
BOOST_AUTO_TEST_CASE(test_readMessage) {
    TestClient client;

    // Prepare the mock data
    QByteArray mockData("Driver:123.456");

    // Get the socket and inject the data
    QTcpSocket* socket = client.getSocket().get();
    QSignalSpy spy(&client, &Client::responseReceived);

    // Simulate the socket being ready to read data
    QMetaObject::invokeMethod(socket, [&mockData, socket]() {
            socket->setProperty("mockData", QVariant(mockData));
            emit socket->readyRead();
        }, Qt::QueuedConnection);

    // Call the readMessage method
    client.readMessage();

    // Verify the response
    BOOST_CHECK_EQUAL(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QString response = arguments.at(0).toString();
    BOOST_CHECK_EQUAL(response.toStdString(), "Driver = 00:02:03:456");
}
