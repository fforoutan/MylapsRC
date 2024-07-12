#define BOOST_TEST_MODULE test_app
#include <boost/test/unit_test.hpp>
#include "driver.h"


BOOST_AUTO_TEST_CASE(DefaultConstructorTest)
{
    Driver driver;
    BOOST_CHECK_EQUAL(driver.GetBestLapTime(), std::numeric_limits<double>::max()); // Since there are no lap times, the best lap time is 0.0
}

BOOST_AUTO_TEST_CASE(ConstructorTest)
{
    Driver driver(42);
    BOOST_CHECK_EQUAL(driver.GetBestLapNumber(), -1); // Since there are no lap times, the best lap number is 0
}

BOOST_AUTO_TEST_CASE(AddLapTimeTest)
{
    Driver driver(42);
    driver.AddLapTime(120.5);
    driver.AddLapTime(130.5);
    driver.AddLapTime(149.8);

    BOOST_CHECK_EQUAL(driver.GetBestLapNumber(), 1);
    BOOST_CHECK_CLOSE(driver.GetBestLapTime(), 10.0, 0.001);
}

BOOST_AUTO_TEST_CASE(GetBestLapTimeTest)
{
    Driver driver(42);
    driver.AddLapTime(120.5);
    driver.AddLapTime(125.0);
    driver.AddLapTime(129.3);

    BOOST_CHECK_CLOSE(driver.GetBestLapTime(), 4.3, 0.001); // Best lap time should be 115.0
}

BOOST_AUTO_TEST_CASE(GetAverageLapTimeTest)
{
    Driver driver(42);
    driver.AddLapTime(120.5);
    driver.AddLapTime(125.0);
    driver.AddLapTime(139.8);

    BOOST_CHECK_CLOSE(driver.GetAverageTime(), 9.65, 0.001); // Average lap time should be approximately 118.433
}

BOOST_AUTO_TEST_CASE(EdgeCasesTest)
{
    Driver driver(42);

    // No lap times added
    BOOST_CHECK_EQUAL(driver.GetBestLapNumber(), -1);
    BOOST_CHECK_EQUAL(driver.GetBestLapTime(), std::numeric_limits<double>::max());
    BOOST_CHECK_EQUAL(driver.GetAverageTime(), 0.0);

    // Adding a single lap time
    driver.AddLapTime(100.0);
    BOOST_CHECK_EQUAL(driver.GetBestLapNumber(), -1);
    BOOST_CHECK_CLOSE(driver.GetBestLapTime(), std::numeric_limits<double>::max(), 0.001);
    BOOST_CHECK_CLOSE(driver.GetAverageTime(), 0.0, 0.001);
}
