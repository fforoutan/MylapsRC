#define BOOST_TEST_MODULE test_app
#include <boost/test/unit_test.hpp>
#include <message_handler.h>

BOOST_AUTO_TEST_CASE(IsAverageTimeRequestTest){
    message_handler message;
    BOOST_CHECK(message.IsAverageTimeRequest("AVG:"));
    BOOST_CHECK(!message.IsAverageTimeRequest("A V G:"));
    BOOST_CHECK(!message.IsAverageTimeRequest("avg:"));

}
BOOST_AUTO_TEST_CASE(ExtractDriverNumberTest)
{
    message_handler handler;
    BOOST_CHECK_EQUAL(handler.ExtractDriverNumber("AVG:1234"), 1234);
    BOOST_CHECK_EQUAL(handler.ExtractDriverNumber("AVG:5678"), 5678);
}

BOOST_AUTO_TEST_CASE(ParseTimeTest)
{
    message_handler handler;
    BOOST_CHECK_CLOSE(handler.ParseTime("01:02:03"), 3723.0, 0.001); // 1 hour, 2 minutes, 3 seconds = 3600 + 120 + 3 = 3723 seconds
    BOOST_CHECK_CLOSE(handler.ParseTime("00:45:30"), 2730.0, 0.001); // 45 minutes, 30 seconds = 2700 + 30 = 2730 seconds
}
BOOST_AUTO_TEST_CASE(ParseRaceDataTest)
{
    message_handler handler;
    std::string race_data = "1,01:02:03\n2,00:45:30\n3,00:30:15";
    std::vector<LapTime> lap_times = handler.ParseRaceData(race_data);
    BOOST_CHECK_EQUAL(lap_times.size(), 3);

    BOOST_CHECK_EQUAL(lap_times[0].driver_number, 1);
    BOOST_CHECK_CLOSE(lap_times[0].time_in_seconds, 3723.0, 0.001);

    BOOST_CHECK_EQUAL(lap_times[1].driver_number, 2);
    BOOST_CHECK_CLOSE(lap_times[1].time_in_seconds, 2730.0, 0.001);

    BOOST_CHECK_EQUAL(lap_times[2].driver_number, 3);
    BOOST_CHECK_CLOSE(lap_times[2].time_in_seconds, 1815.0, 0.001); // 30 minutes, 15 seconds = 1800 + 15 = 1815 seconds
}

BOOST_AUTO_TEST_CASE(ParseRaceDataEmptyTest)
{
    message_handler handler;
    std::string race_data = "";
    std::vector<LapTime> lap_times = handler.ParseRaceData(race_data);
    BOOST_CHECK_EQUAL(lap_times.size(), 0);
}
