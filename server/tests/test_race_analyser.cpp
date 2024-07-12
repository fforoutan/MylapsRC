#define BOOST_TEST_MODULE test_app
#include <boost/test/unit_test.hpp>
#include <race_analyser.h>
#include <fstream>


BOOST_AUTO_TEST_CASE(DefaultConstructorTest)
{
    race_analyser analyser(5);
    BOOST_CHECK_EQUAL(analyser.CalculateWinner(), "No driver completed all laps.\n");
}

BOOST_AUTO_TEST_CASE(AddLapTimesTest)
{
    race_analyser analyser(5);
    std::vector<LapTime> lap_times = {
        {1, 60.5},
        {2, 61.5},
        {1, 62.2},
        {2, 63.3},
        {1, 64.4},
        {2, 65.5},
        {1, 66.5},
        {2, 68.5},
        {1, 69.2},
        {2, 80.3},
        {1, 82.2},
        {2, 82.3}
    };
    analyser.AddLapTimes(lap_times);
    BOOST_CHECK_EQUAL(analyser.CalculateWinner(), "Race Results:\n\n1st: Driver 1 : 1.7s\n"
                                                  "2nd: Driver 2 : +0.1s\n");
}
BOOST_AUTO_TEST_CASE(FailedAddLapTimesTest)
{
    race_analyser analyser(5);
    std::vector<LapTime> lap_times = {
                                      {1, 60.5},
                                      {2, 61.5},
                                      {1, 62.2},
                                      {2, 63.3},
                                      };
    analyser.AddLapTimes(lap_times);
    BOOST_CHECK_EQUAL(analyser.CalculateWinner(), "No driver completed all laps.\n");
}

BOOST_AUTO_TEST_CASE(ProcessRaceDataTest)
{
    race_analyser analyser(5);
    std::vector<LapTime> lap_times = {
        {1, 60.5},
        {1, 61.2},
        {1, 64.5},
        {1, 66.7},
        {1, 68.9},
        {1, 69.9}
    };
    analyser.AddLapTimes(lap_times);
    BOOST_CHECK_EQUAL(analyser.CalculateWinner(), "Race Results:\n\n1st: Driver 1 : 0.7s\n");
}

BOOST_AUTO_TEST_CASE(CalculateWinnerTest)
{
    race_analyser analyser(5);
    std::vector<LapTime> lap_times = {
        {1, 60.5},
        {1, 61.3},
        {1, 62.3},
        {1, 63.4},
        {1, 64.5},
        {1, 65.5},
        {2, 60.6},
        {2, 61.3},
        {2, 62.4},
        {2, 63.5},
        {2, 64.6},
        {2, 65.6}
    };
    analyser.AddLapTimes(lap_times);
    BOOST_CHECK_EQUAL(analyser.CalculateWinner(), "Race Results:\n\n1st: Driver 2 : 0.7s\n2nd: Driver 1 : +0.1s\n");
}

BOOST_AUTO_TEST_CASE(GetAverageTimeTest)
{
    race_analyser analyser(5);
    std::vector<LapTime> lap_times = {
        {1, 60.5},
        {1, 61.2},
        {1, 62.3},
        {1, 63.4},
        {1, 64.5},
        {1, 65.5}
    };
    analyser.AddLapTimes(lap_times);
    BOOST_CHECK_EQUAL(analyser.GetAverageTime(1), "Driver 1 average lap time: 1\n");
    BOOST_CHECK_EQUAL(analyser.GetAverageTime(2), "Driver not found.\n");
}

BOOST_AUTO_TEST_CASE(GetPostfixTest)
{
    race_analyser analyser(5);
    BOOST_CHECK_EQUAL(analyser.getPostfix(1), "st");
    BOOST_CHECK_EQUAL(analyser.getPostfix(2), "nd");
    BOOST_CHECK_EQUAL(analyser.getPostfix(3), "rd");
    BOOST_CHECK_EQUAL(analyser.getPostfix(4), "th");
    BOOST_CHECK_EQUAL(analyser.getPostfix(11), "th");
    BOOST_CHECK_EQUAL(analyser.getPostfix(12), "th");
    BOOST_CHECK_EQUAL(analyser.getPostfix(13), "th");
    BOOST_CHECK_EQUAL(analyser.getPostfix(21), "st");
    BOOST_CHECK_EQUAL(analyser.getPostfix(22), "nd");
    BOOST_CHECK_EQUAL(analyser.getPostfix(23), "rd");
}
