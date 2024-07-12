#include "message_handler.h"
#include <sstream>
#include <iostream>
//#include <iomanip>
#include "server.h"

bool message_handler::IsAverageTimeRequest(const std::string& message) const {
    return message.rfind("AVG:", 0) == 0;
}

int message_handler::ExtractDriverNumber(const std::string& message) const {
    return std::stoi(message.substr(4));
}

std::vector<LapTime> message_handler::ParseRaceData(const std::string& message) const {
    std::vector<LapTime> lap_times;
    std::stringstream ss(message);
    std::string line;
    int driver_number = -1;
    double time_in_seconds = -1;
    lap_times.clear();
    while (std::getline(ss, line)) {
        std::stringstream line_stream(line);
        std::string driver_number_str, time_str;
        if (std::getline(line_stream, driver_number_str, ',') && std::getline(line_stream, time_str, ',')) {
            driver_number = std::stoi(driver_number_str);
            time_in_seconds = ParseTime(time_str);
            lap_times.push_back({driver_number, time_in_seconds});
        }
    }
    return lap_times;
}

double message_handler::ParseTime(const std::string& time_str) const {
    int hours, minutes, seconds;
    char delimiter;
    std::stringstream ss(time_str);
    ss >> hours >> delimiter >> minutes >> delimiter >> seconds;
    return hours * 3600 + minutes * 60 + seconds;
}
