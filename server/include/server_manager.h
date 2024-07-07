#ifndef SERVER_MANAGER_H
#define SERVER_MANAGER_H

#include <memory>
#include "server_socket.h"
#include "race_analyser.h"
#include "message_handler.h"

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

private:
    /**
     * @brief Sets the message handler function.
     * @param handler The function to handle incoming messages.
     */
    void HandleMessage(const std::string& message, SOCKET client_socket);

    std::unique_ptr<server_socket> server_socket_;
    std::unique_ptr<message_handler> message_handler_;
    std::unordered_map<SOCKET, std::unique_ptr<race_analyser>> client_data_;
    int total_laps_;

};

#endif  // SERVER_MANAGER_H
