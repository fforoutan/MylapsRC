#include "server_manager.h"

server_manager::server_manager(int port, int total_laps)
    : server_socket_(std::make_unique<server_socket>(port)),
    total_laps_(total_laps) {
    server_socket_->SetMessageHandler([this](const std::string& message, SOCKET client_socket) {
        this->HandleMessage(message, client_socket);
    });
}

void server_manager::Start() {
    server_socket_->Run();
}

void server_manager::HandleMessage(const std::string& message, SOCKET client_socket) {


 }
