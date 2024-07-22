#ifndef SERVER_MANAGER_H
#define SERVER_MANAGER_H

#include <memory>
#include <unordered_map>
#include "server_socket.h"
#include "race_analyser.h"
#include "message_analyser.h"

#ifdef USE_BOOST
#include <boost/asio.hpp>
#endif

/**
 * @brief The ServerManager class manages client connections and race data.
 */
class server_manager {
public:
    /**
     * @brief Constructs a ServerManager object.
     * @param port The port number on which the server listens.
     * @param total_laps The total number of laps in the race.
     */
    server_manager(int port, int total_laps);
    /**
     * @brief Starts the server to listen for incoming connections.
     */
    void Start();

    /**
     * @brief Stops the server .
     */
    void Stop();

    /**
     * @brief Sets the message handler function.
     * @param handler The function to handle incoming messages.
     */
#ifdef USE_BOOST
    void HandleMessage(const std::string& message, std::shared_ptr<boost::asio::ip::tcp::socket> client_socket);
#else
    void HandleMessage(const std::string& message, SOCKET client_socket);
#endif

private:
    std::unique_ptr<server_socket> server_socket_;
    std::unique_ptr<message_analyser> message_analyser_;
#ifdef USE_BOOST
    std::unordered_map<std::shared_ptr<boost::asio::ip::tcp::socket>, std::unique_ptr<race_analyser>> client_data_;
#else
    std::unordered_map<SOCKET, std::unique_ptr<race_analyser>> client_data_;
#endif
    int total_laps_;
};

#endif  // SERVER_MANAGER_H
