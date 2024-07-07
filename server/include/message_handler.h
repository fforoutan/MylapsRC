#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <string>
#include <vector>
#include "race_analyser.h"

/**
 * @brief The message_handler class provides methods to analyze and interpret incoming messages.
 */
class message_handler {
public:
    /**
     * @brief Checks if the message is a request for the average time of a driver.
     * @param message The incoming message.
     * @return True if the message is an average time request, false otherwise.
     */
    bool IsAverageTimeRequest(const std::string& message) const;

    /**
     * @brief Extracts the driver number from an average time request message.
     * @param message The incoming message.
     * @return The driver number.
     */
    int ExtractDriverNumber(const std::string& message) const;

    /**
     * @brief Parses race data from the incoming message.
     * @param message The incoming message.
     * @return A vector of LapTime objects containing the parsed race data.
     */
    std::vector<LapTime> ParseRaceData(const std::string& message) const;

    /**
     * @brief Converts a time string in HH:MM:SS format to seconds.
     * @param time_str The time string.
     * @return The time in seconds.
     */
    double ParseTime(const std::string& time_str) const;
};

#endif  // MESSAGE_HANDLER_H
