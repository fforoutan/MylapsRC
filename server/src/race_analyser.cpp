#include "race_analyser.h"
#include <sstream>
#include <limits>
#include <algorithm>

race_analyser::race_analyser(int total_laps) : total_laps_(total_laps) {}

void race_analyser::AddLapTimes(const std::vector<LapTime>& lap_times) {
    ProcessRaceData(lap_times);
}

void race_analyser::ProcessRaceData(const std::vector<LapTime>& lap_times) {
    race_data_.clear();
    for (const auto& lap : lap_times) {
        if (race_data_.find(lap.driver_number) == race_data_.end()) {
            race_data_[lap.driver_number] = Driver(lap.driver_number);
        }
        race_data_[lap.driver_number].AddLapTime(lap.time_in_seconds);
    }

}

std::string race_analyser::getPostfix(int rank)
{
    if (rank % 10 == 1 && rank % 100 != 11)
        return "st";
    else if (rank % 10 == 2 && rank % 100 != 12)
        return "nd";
    else if (rank % 10 == 3 && rank % 100 != 13)
        return "rd";
    else
        return "th";
}

std::string race_analyser::CalculateWinner() {
    double winner_fastest_time = std::numeric_limits<double>::max();
    int winner_driver_number = -1;
    int winner_fastest_lap = 0;
    for (const auto& entry : race_data_) {
        const Driver& driver = entry.second;
        if (driver.GetTotalLapsNumber() < total_laps_) {
            continue;
        }
        printf("driver number:%d, best time:%f, best lap:%d, avg time:%f, total:%f \n",
               driver.GetDriverNumber(), driver.GetBestLapTime(), driver.GetBestLapNumber(),
               driver.GetAverageTime(), driver.GetTotalTime(), driver.GetTotalLapsNumber());
        if (driver.GetBestLapTime() < winner_fastest_time){
            winner_driver_number = driver.GetDriverNumber();
            winner_fastest_time = driver.GetBestLapTime();
            winner_fastest_lap = driver.GetBestLapNumber();
        } else if (driver.GetBestLapTime() == winner_fastest_time) {
            if (driver.GetBestLapNumber() < winner_fastest_lap){
                winner_driver_number = driver.GetDriverNumber();
                winner_fastest_time = driver.GetBestLapTime();
                winner_fastest_lap = driver.GetBestLapNumber();
            }
        }
    }

    if (winner_driver_number == -1) {
        return "No driver completed all laps.\n";
    }
    std::vector<std::tuple<int, double, double>> driver_times;
    for (const auto &entry : race_data_) {
        int driver_number = entry.first;
        const Driver& driver = entry.second;
        driver_times.push_back({driver_number, driver.GetBestLapNumber(), driver.GetBestLapTime() - winner_fastest_time});
    }

    std::sort(driver_times.begin(), driver_times.end(), [](const auto &a, const auto &b) {
        return std::get<2>(a) < std::get<2>(b);
    });
    std::sort(driver_times.begin(), driver_times.end(), [](const auto &a, const auto &b) {
        if (std::get<2>(a) == std::get<2>(b))
            return std::get<1>(a) < std::get<1>(b);
    });

    std::ostringstream oss;
    oss << "Race Results:\n\n";
    oss << "1st: Driver " << winner_driver_number << " : " << winner_fastest_time << "s\n";
    for (size_t i = 1; i < driver_times.size(); ++i) {
        int driver_number = std::get<0>(driver_times[i]);
        double total_time = std::get<2>(driver_times[i]);
        oss << i + 1 << getPostfix(i+1) << ": Driver " << driver_number << " : +" << total_time << "s\n";
    }
    race_results_ = oss.str();
    return race_results_;
}

std::string race_analyser::GetAverageTime(int driver_number) const {
    auto it = race_data_.find(driver_number);
    if (it == race_data_.end()) {
        return "Driver not found.\n";
    }

    const Driver& driver = it->second;
    double avg_time = driver.GetBestLapTime();
    std::ostringstream oss;
    oss << "Driver " << driver_number << " average lap time: " << avg_time << "\n";
    return oss.str();
}
