#ifndef SERVER_H
#define SERVER_H

#include <string>

/**
 * @brief The Server class represents a server that can load configuration settings and validate them.
 */
class Server {
public:
    /**
     * @brief Constructs a Server object with default IP and port settings.
     */
    Server();

    /**
     * @brief Loads settings from the configuration file and validates them.
     * @param config_path The path to the configuration file.
     */
    void LoadSettings(const std::string& config_path);

    /**
     * @brief Gets the loaded IP address.
     * @return The IP address as a string.
     */
    std::string GetIP() const;

    /**
     * @brief Gets the loaded port number.
     * @return The port number as an integer.
     */
    int GetPort() const;
    /**
     * @brief Validates the given IP address.
     * @param ip The IP address to validate.
     * @return True if the IP address is valid, false otherwise.
     */
    bool IsValidIPAddress(const std::string& ip);

    /**
     * @brief Validates the given port number.
     * @param port The port number to validate.
     * @return True if the port number is valid, false otherwise.
     */
    bool IsValidPort(int port);


private:
    std::string ip_;  ///< The IP address.
    int port_;  ///< The port number.


};

#endif // SERVER_H
