#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H
#ifdef USE_BOOST
#include <boost/asio.hpp>
#else
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#endif

#include <functional>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <queue>

#ifdef USE_BOOST
using tcp_socket = std::shared_ptr<boost::asio::ip::tcp::socket>;
#else
using tcp_socket = SOCKET;
#endif
/**
 * @brief The server_socket class manages the socket communication for the server.
 */
class server_socket {
public:
    /**
     * @brief Constructs a server_socket object and initializes the server.
     * @param port The port number on which the server listens.
     */
    explicit server_socket(int port);

    /**
     * @brief Destructs the server_socket object and cleans up resources.
     */
    ~server_socket();

    /**
     * @brief Starts the server to listen for incoming connections.
     */
    void Run();

    void Stop();
    /**
     * @brief Sets the message handler function.
     * @param handler The function to handle incoming messages.
     */
#ifdef USE_BOOST
    void SetMessageHandler(const std::function<void(const std::string&, tcp_socket)>& handler);
#else
    void SetMessageHandler(const std::function<void(const std::string&, SOCKET)>& handler);
#endif

private:
    int port_; ///< The port number on which the server listens.
#ifdef USE_BOOST
    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
#else
    tcp_socket listen_socket_;///< The listening socket for incoming connections.
#endif
    bool running_; ///< Flag indicating whether the server is running.

///< The function to handle incoming messages.
#ifdef USE_BOOST
    std::function<void(const std::string&, tcp_socket&)> message_handler_;
#else
    std::function<void(const std::string&, tcp_socket)> message_handler_;
#endif

    std::thread accept_thread_; ///< Thread for accepting new connections.
    std::vector<std::thread> worker_threads_; ///< Threads for processing client messages.
    std::mutex queue_mutex_; ///< Mutex for synchronizing access to the message queue.
    std::condition_variable condition_; ///< Condition variable for worker thread synchronization.
    std::queue<std::pair<tcp_socket, std::string>> message_queue_; ///< Queue for storing client messages.

    /**
     * @brief Sets up the server by initializing the socket and binding to the port.
     */
    void SetupServer();

    /**
     * @brief Accepts new client connections.
     */
    void AcceptConnections();

    /**
     * @brief Processes client messages in a worker thread.
     */
    void WorkerThread();

    /**
     * @brief Handles a client connection.
     * @param client_socket The socket of the connected client.
     */
    void HandleClient(tcp_socket client_socket);
};

#endif  // SERVER_SOCKET_H
