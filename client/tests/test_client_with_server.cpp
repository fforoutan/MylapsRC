#include "include/client.h"
#include "mock_tcpserver.h"
#include <boost/test/unit_test.hpp>
#include <QSignalSpy>
#include <QByteArray>
#include <QTest>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace ::testing;

class ClientWithServerTest : public ::testing::Test {
protected:
    void SetUp() override {
        server = new MockTcpServer();
        client = new Client(nullptr);

        // Start the mock server
        server->startServer(12345);

        // Wait for the server to start
        QTest::qWait(100);
    }

    void TearDown() override {
        delete client;
        delete server;
    }

    MockTcpServer *server;
    Client *client;
};

TEST_F(ClientWithServerTest, TestClientServerInteraction) {
    // Connect the client to the mock server
    client->requestAverageTime("1");

    // Wait for the client to connect
    QTest::qWait(100);

    // Get the client socket from the server
    QTcpSocket *clientSocket = server->getClientSocket();
    ASSERT_NE(clientSocket, nullptr);

    // Prepare the mock data to send to the client
    QByteArray mockData("Driver:123.456");

    // Send the mock data to the client
    clientSocket->write(mockData);
    clientSocket->flush();

    // Wait for the client to process the data
    QTest::qWait(100);

    // Use QSignalSpy to verify the response
    QSignalSpy spy(client, &Client::responseReceived);
    ASSERT_EQ(spy.count(), 1);

    QList<QVariant> arguments = spy.takeFirst();
    QString response = arguments.at(0).toString();
    ASSERT_EQ(response.toStdString(), "Driver = 00:02:03:456");
}
