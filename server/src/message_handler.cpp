#include "message_handler.h"
#include <sstream>
#include <iostream>
#include <iomanip>

bool message_handler::IsAverageTimeRequest(const std::string& message) const {
    return message.rfind("AVG:", 0) == 0;
}

int message_handler::ExtractDriverNumber(const std::string& message) const {
    return std::stoi(message.substr(4));
}


double message_handler::ParseTime(const std::string& time_str) const {
    int hours, minutes, seconds;
    char delimiter;
    std::stringstream ss(time_str);
    ss >> hours >> delimiter >> minutes >> delimiter >> seconds;
    return hours * 3600 + minutes * 60 + seconds;
}
