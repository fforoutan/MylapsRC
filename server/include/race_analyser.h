#ifndef RACE_ANALYSER_H
#define RACE_ANALYSER_H

#include "driver.h"
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief The LapTime struct represents a lap time for a driver.
 */
struct LapTime {
    int driver_number; ///< The number of the driver.
    double time_in_seconds; ///< The lap time in seconds.
};

/**
 * @brief The race_analyser class analyzes race data and determines the winner.
 */
class race_analyser {
public:
    /**
     * @brief Constructs a race_analyser object.
     * @param total_laps The total number of laps in the race.
     */
    explicit race_analyser(int total_laps);

    /**
     * @brief Adds lap times to the race data.
     * @param lap_times A vector of LapTime objects containing the lap times to add.
     */
    void AddLapTimes(const std::vector<LapTime>& lap_times);

    /**
     * @brief Calculates the winner of the race based on the lap times.
     * @return A string containing the race results and the winner.
     */
    std::string CalculateWinner();

    /**
     * @brief Gets the average lap time for a specific driver.
     * @param driver_number The number of the driver.
     * @return A string containing the average lap time of the driver.
     */
    std::string GetAverageTime(int driver_number) const;

private:
    int total_laps_; ///< The total number of laps in the race.
    std::unordered_map<int, Driver> race_data_; ///< The race data, mapping driver numbers to their lap times.
    std::string race_results_; ///< The results of the race.

    /**
     * @brief Processes the lap times and updates the race data.
     * @param lap_times A vector of LapTime objects containing the lap times to process.
     */
    void ProcessRaceData(const std::vector<LapTime>& lap_times);

    /**
     * @brief Gets the appropriate postfix for a given rank.
     * @param rank The rank for which to get the postfix.
     * @return A string containing the postfix (e.g., "st", "nd", "rd", "th").
     */
    std::string getPostfix(int rank);
};

#endif  // RACE_ANALYSER_H
