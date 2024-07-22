#include <iostream>
#include "server.h"
#include "server_manager.h"
#include <iostream>
#include <csignal>
#include <thread>
#include <atomic>
std::atomic<bool> running(true);

void signal_handler(int signal) {
    std::cout << "Interrupt signal (" << signal << ") received. Shutting down server." << std::endl;
    running = false;
}

int main() {
    Server server;
    server.LoadSettings(std::string(SOURCE_DIR) + "/resources/config/config.ini");

    std::string ip = server.GetIP();
    int port = server.GetPort();
    server_manager manager(port, 4);

    std::signal(SIGINT, signal_handler);

    std::cout << "Server starting on port " << port << "..." << std::endl;
    std::thread server_thread([&manager]() {
        manager.Start();
    });

    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    manager.Stop();
    server_thread.join();

    std::cout << "Server shut down gracefully." << std::endl;

}
