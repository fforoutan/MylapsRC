#include "driver.h"
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdio.h>
Driver::Driver()
    : driver_number_(-1),
    best_lap_time_(std::numeric_limits<double>::max()),
    best_lap_number_(-1),
    total_time_(0.0) {}

Driver::Driver(int driver_number)
    : driver_number_(driver_number),
    best_lap_time_(std::numeric_limits<double>::max()),
    best_lap_number_(-1),
    total_time_(0.0) {}

void Driver::AddLapTime(double lap_day_time) {

    lap_times_.push_back(lap_day_time);
    if (lap_times_.size() > 1) {
        double lap_time = lap_times_.back() - lap_times_.at(lap_times_.size() - 2);
        total_time_ += lap_time;
        if (lap_time < best_lap_time_) {
            best_lap_time_ = lap_time;
            best_lap_number_ = lap_times_.size();
        }
    }

}

double Driver::GetBestLapTime() const {
    return best_lap_time_;
}

int Driver::GetBestLapNumber() const {
    return best_lap_number_;
}

double Driver::GetAverageTime() const {
    if (lap_times_.empty()) {
        return 0.0;
    }
    return total_time_ / lap_times_.size();
}

double Driver::GetTotalTime() const {
    return total_time_;
}

int Driver::GetDriverNumber() const {
    return driver_number_;
}

std::vector<double> Driver::GetLapTimes() const
{
    return lap_times_;
}

int Driver::GetTotalLapsNumber() const
{
    return lap_times_.size();
}
