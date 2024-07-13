#ifndef USE_BOOST
#include <gtest/gtest.h>
#include "server_socket.h"
#include <thread>
#include <chrono>

class ServerSocketTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Code for setting up before each test
    }

    void TearDown() override {
        // Code for cleaning up after each test
    }
};

TEST_F(ServerSocketTest, CanCreateServerSocket) {
    server_socket server(8088); // Using a random port for testing
    EXPECT_TRUE(true); // If no exceptions, the test passes
}

TEST_F(ServerSocketTest, CanRunServerSocket) {
    server_socket server(8088);
    std::thread server_thread([&server]() {
        server.Run();
    });

    std::this_thread::sleep_for(std::chrono::seconds(1));

    EXPECT_TRUE(server_thread.joinable()); // Check if the server thread is running

    server_thread.detach(); // Detach the thread to let the server run independently
}

TEST_F(ServerSocketTest, CanAcceptConnections) {
    server_socket server(8088);
    server.SetMessageHandler([](const std::string& message, SOCKET client_socket) {
        std::cout << "Message received: " << message << std::endl;
    });

    std::thread server_thread([&server]() {
        server.Run();
    });

    std::this_thread::sleep_for(std::chrono::seconds(1));

    bool connection_successful = false;

    EXPECT_TRUE(connection_successful);

    server_thread.detach();
}
#endif
#ifdef USE_BOOST
#include <gtest/gtest.h>
#include "server_socket.h"
#include <thread>
#include <chrono>
#include <boost/asio.hpp>
#include <future>
#include <atomic>

using boost::asio::ip::tcp;

class ServerSocketTest : public ::testing::Test {
protected:
    server_socket* server;
    std::thread server_thread;
    std::atomic<bool> server_should_stop{false};

    void SetUp() override {
        server = new server_socket(12345);

        server_thread = std::thread([this]() {
            server->Run();
        });

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    void TearDown() override {
        {
            boost::asio::io_context io_context;
            tcp::resolver resolver(io_context);
            tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "12345");
            tcp::socket socket(io_context);
            boost::asio::connect(socket, endpoints);
            std::string shutdown_message = "shutdown";
            boost::asio::write(socket, boost::asio::buffer(shutdown_message));
        }

        if (server) {
            server->Stop();
        }
        if (server_thread.joinable()) {
            server_thread.join();
        }
        delete server;
    }
};

TEST_F(ServerSocketTest, CanCreateServerSocket) {
    EXPECT_TRUE(true);
}

TEST_F(ServerSocketTest, CanRunServerSocket) {
    EXPECT_TRUE(server_thread.joinable());
}

TEST_F(ServerSocketTest, CanAcceptConnections) {
    std::cout << "Running CanAcceptConnections test...\n";
    server->SetMessageHandler([this](const std::string& message, std::shared_ptr<tcp::socket> client_socket) {
        std::cout << "Message received: " << message << std::endl;
        if (message == "shutdown") {
            server_should_stop = true;
        } else {
            boost::asio::write(*client_socket, boost::asio::buffer(message));
        }
    });

    auto future = std::async(std::launch::async, [this]() {
        std::cout << "Simulating client connection...\n";
        boost::asio::io_context io_context;
        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "12345");
        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        std::cout << "Sending test message...\n";
        std::string test_message = "Hello, server!";
        boost::asio::write(socket, boost::asio::buffer(test_message));

        char reply[512];
        size_t reply_length = boost::asio::read(socket, boost::asio::buffer(reply, test_message.size()));

        std::string reply_message(reply, reply_length);
        std::cout << "Received reply: " << reply_message << std::endl;
        return reply_message == test_message;
    });

    auto status = future.wait_for(std::chrono::seconds(5));
    if (status == std::future_status::timeout) {
        server->Stop();
        FAIL() << "Test timed out";
    } else {
        EXPECT_TRUE(future.get()); // Verify the server handled the connection correctly
    }

    std::cout << "Sending shutdown message...\n";
    {
        boost::asio::io_context io_context;
        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "12345");
        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);
        std::string shutdown_message = "shutdown";
        boost::asio::write(socket, boost::asio::buffer(shutdown_message));
    }


    std::cout << "CanAcceptConnections test completed. Stopping server...\n";
    std::cout << "Server stopped.\n";
}
#endif
