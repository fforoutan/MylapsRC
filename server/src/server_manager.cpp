#include "server_manager.h"

server_manager::server_manager(int port, int total_laps)
    : server_socket_(std::make_unique<server_socket>(port)),
    total_laps_(total_laps),
    message_handler_(std::make_unique<message_handler>()) {
    server_socket_->SetMessageHandler([this](const std::string& message, SOCKET client_socket) {
        if (client_data_.find(client_socket) == client_data_.end()) {
            client_data_[client_socket] = std::make_unique<race_analyser>(total_laps_);
        }
        this->HandleMessage(message, client_socket);
    });
}

void server_manager::Start() {
    server_socket_->Run();
}

void server_manager::HandleMessage(const std::string& message, SOCKET client_socket) {
    auto& race_analyser = client_data_[client_socket];
    if (message_handler_->IsAverageTimeRequest(message)) {
        int driver_number = message_handler_->ExtractDriverNumber(message);
        std::string response = race_analyser->GetAverageTime(driver_number);
        send(client_socket, response.c_str(), response.length(), 0);
    } else {
        auto lap_times = message_handler_->ParseRaceData(message);
        printf("lap_times size:", lap_times.size());
        race_analyser->AddLapTimes(lap_times);
        std::string response = race_analyser->CalculateWinner();
        send(client_socket, response.c_str(), response.length(), 0);
    }
}
