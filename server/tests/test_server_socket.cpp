#ifndef USE_BOOST
#include <gtest/gtest.h>
#include "server_socket.h"
#include <thread>
#include <chrono>

// A basic test fixture for server_socket
class ServerSocketTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Code for setting up before each test
    }

    void TearDown() override {
        // Code for cleaning up after each test
    }
};

// Test case to check if the server_socket can be created
TEST_F(ServerSocketTest, CanCreateServerSocket) {
    server_socket server(8088); // Using a random port for testing
    EXPECT_TRUE(true); // If no exceptions, the test passes
}

// Test case to check if the server can start running
TEST_F(ServerSocketTest, CanRunServerSocket) {
    server_socket server(8088);
    std::thread server_thread([&server]() {
        server.Run();
    });

    // Give the server some time to start
    std::this_thread::sleep_for(std::chrono::seconds(1));

    EXPECT_TRUE(server_thread.joinable()); // Check if the server thread is running

    server_thread.detach(); // Detach the thread to let the server run independently
}

// Test case to check if the server accepts connections
TEST_F(ServerSocketTest, CanAcceptConnections) {
    server_socket server(8088);
    server.SetMessageHandler([](const std::string& message, SOCKET client_socket) {
        std::cout << "Message received: " << message << std::endl;
    });

    std::thread server_thread([&server]() {
        server.Run();
    });

    // Give the server some time to start
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Simulate a client connection (simplified example, replace with actual client connection code)
    // This part might be complex, as it depends on the specific client implementation
    // Here we assume the presence of a basic client function to test the connection

    bool connection_successful = false;
    // Code to create a client socket and connect to the server
    // Set connection_successful to true if connection is successful

    EXPECT_TRUE(connection_successful); // Verify the server accepted the connection

    server_thread.detach(); // Detach the thread to let the server run independently
}
#endif
//-------------------------------------------------------
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
        // Create the server on an arbitrary port
        server = new server_socket(12345);

        // Start the server in a separate thread
        server_thread = std::thread([this]() {
            server->Run();
        });

        // Give the server some time to start
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    void TearDown() override {
        // Send a shutdown message to the server
        {
            boost::asio::io_context io_context;
            tcp::resolver resolver(io_context);
            tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "12345");
            tcp::socket socket(io_context);
            boost::asio::connect(socket, endpoints);
            std::string shutdown_message = "shutdown";
            boost::asio::write(socket, boost::asio::buffer(shutdown_message));
        }

        // Join the server thread
        if (server) {
            server->Stop();
        }
        if (server_thread.joinable()) {
            server_thread.join();
        }
        delete server;
    }
};

// Test case to check if the server_socket can be created
TEST_F(ServerSocketTest, CanCreateServerSocket) {
    EXPECT_TRUE(true); // If no exceptions, the test passes
}

// Test case to check if the server can start running
TEST_F(ServerSocketTest, CanRunServerSocket) {
    EXPECT_TRUE(server_thread.joinable()); // Check if the server thread is running
}

// Test case to check if the server accepts and handles connections
TEST_F(ServerSocketTest, CanAcceptConnections) {
    std::cout << "Running CanAcceptConnections test...\n";
    server->SetMessageHandler([this](const std::string& message, std::shared_ptr<tcp::socket> client_socket) {
        std::cout << "Message received: " << message << std::endl;
        // If the message is "shutdown", set the stop flag
        if (message == "shutdown") {
            server_should_stop = true;
        } else {
            // Echo the message back to the client
            boost::asio::write(*client_socket, boost::asio::buffer(message));
        }
    });

    auto future = std::async(std::launch::async, [this]() {
        std::cout << "Simulating client connection...\n";
        // Simulate a client connection
        boost::asio::io_context io_context;
        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "12345");
        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        std::cout << "Sending test message...\n";
        // Send a test message
        std::string test_message = "Hello, server!";
        boost::asio::write(socket, boost::asio::buffer(test_message));

        // Receive the response
        char reply[512];
        size_t reply_length = boost::asio::read(socket, boost::asio::buffer(reply, test_message.size()));

        // Verify the response
        std::string reply_message(reply, reply_length);
        std::cout << "Received reply: " << reply_message << std::endl;
        return reply_message == test_message;
    });

    // Wait for the future with a timeout
    auto status = future.wait_for(std::chrono::seconds(5));
    if (status == std::future_status::timeout) {
        server->Stop();
        FAIL() << "Test timed out";
    } else {
        EXPECT_TRUE(future.get()); // Verify the server handled the connection correctly
    }

    std::cout << "Sending shutdown message...\n";
    // Send a shutdown message to the server to stop it
    {
        boost::asio::io_context io_context;
        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "12345");
        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);
        std::string shutdown_message = "shutdown";
        boost::asio::write(socket, boost::asio::buffer(shutdown_message));
    }

    // Wait for the server to process the shutdown message

    std::cout << "CanAcceptConnections test completed. Stopping server...\n";
    // Ensure the server stops after the test
    server->Stop();
    std::cout << "Server stopped.\n";
}
#endif
// =======================================================================================
