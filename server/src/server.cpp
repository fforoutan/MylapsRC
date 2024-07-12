#include "server.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>

//#ifdef _WIN32
//#include <windows.h>
//#else
//#include <unistd.h>
//#include <limits.h>
//#endif

Server::Server() : ip_("127.0.0.1"), port_(8080) {}

void Server::LoadSettings(const std::string& config_path) {
    std::ifstream file(config_path);
    if (!file.is_open()) {
        std::cerr << "Failed to open configuration file at " << config_path << ". Using default settings." << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::cerr << line << "<<<<<\n";
        std::istringstream line_stream(line);
        std::string key;
        if (std::getline(line_stream, key, '=')) {
             std::cerr << key << "====" << "===\n";
            std::string value;
            std::cerr << value << "*******\n";
            if (std::getline(line_stream, value)) {
                if (key == "IP") {
                    if (IsValidIPAddress(value))
                        ip_ = value;
                    else
                        std::cerr << "Invalid IP address in configuration file. Using default IP: 127.0.0.1" << std::endl;
                } else if (key == "Port") {
                    int temp_port = std::stoi(value);
                    if (IsValidPort(temp_port))
                        port_ = temp_port;
                    else
                        std::cerr << "Invalid port number in configuration file. Using default port: 8088" << std::endl;

                }
            }
        }
    }

    std::cout << "Loaded settings: IP = " << ip_ << ", Port = " << port_ << std::endl;
}

std::string Server::GetIP() const {
    return ip_;
}

int Server::GetPort() const {
    return port_;
}

bool Server::IsValidIPAddress(const std::string& ip) {
    const std::regex ip_regex("(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.)"
                           "{3}([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])");
    return std::regex_match(ip, ip_regex);
}

bool Server::IsValidPort(int port) {
    return port > 0 && port <= 65535;
}
