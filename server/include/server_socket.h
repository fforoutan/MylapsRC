#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

#pragma comment(lib, "Ws2_32.lib")

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

    /**
     * @brief Sets the message handler function.
     * @param handler The function to handle incoming messages.
     */
    void SetMessageHandler(const std::function<void(const std::string&, SOCKET)>& handler);

private:
    int port_; ///< The port number on which the server listens.
    SOCKET listen_socket_; ///< The listening socket for incoming connections.
    bool running_; ///< Flag indicating whether the server is running.
    std::function<void(const std::string&, SOCKET)> message_handler_; ///< The function to handle incoming messages.

    std::thread accept_thread_; ///< Thread for accepting new connections.
    std::vector<std::thread> worker_threads_; ///< Threads for processing client messages.
    std::mutex queue_mutex_; ///< Mutex for synchronizing access to the message queue.
    std::condition_variable condition_; ///< Condition variable for worker thread synchronization.
    std::queue<std::pair<SOCKET, std::string>> message_queue_; ///< Queue for storing client messages.

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
    void HandleClient(SOCKET client_socket);
};

#endif  // SERVER_SOCKET_H
