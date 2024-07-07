#ifndef DRIVER_H
#define DRIVER_H

#include <vector>

/**
 * @brief The Driver class represents a driver in a race.
 */
class Driver {
public:
    /**
     * @brief Constructs a Driver object with default values.
     */
    Driver();

    /**
     * @brief Constructs a Driver object with a specified driver number.
     * @param driver_number The number assigned to the driver.
     */
    Driver(int driver_number);

    /**
     * @brief Adds a lap time to the driver's record.
     * @param lap_time The time taken to complete the lap.
     */
    void AddLapTime(double lap_time);

    /**
     * @brief Gets the best lap time of the driver.
     * @return The best lap time in seconds.
     */
    double GetBestLapTime() const;

    /**
     * @brief Gets the lap number of the best lap time.
     * @return The lap number of the best lap.
     */
    int GetBestLapNumber() const;

    /**
     * @brief Gets the average lap time of the driver.
     * @return The average lap time in seconds.
     */
    double GetAverageTime() const;

    /**
     * @brief Gets the total time taken by the driver to complete all laps.
     * @return The total time in seconds.
     */
    double GetTotalTime() const;

    /**
     * @brief Gets the driver number.
     * @return The driver's number.
     */
    int GetDriverNumber() const;

    /**
     * @brief Gets the lap times of the driver.
     * @return A vector containing the lap times.
     */
    std::vector<double> GetLapTimes() const;

    /**
     * @brief Gets the total number of laps completed by the driver.
     * @return The total number of laps.
     */
    int GetTotalLapsNumber() const;

private:
    int driver_number_; ///< The number assigned to the driver.
    std::vector<double> lap_times_; ///< A vector storing the lap times.
    double best_lap_time_; ///< The best lap time recorded.
    int best_lap_number_; ///< The lap number of the best lap time.
    double total_time_; ///< The total time taken to complete all laps.
};

#endif  // DRIVER_H
