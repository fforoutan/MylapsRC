#define BOOST_TEST_MODULE test_app
#include <boost/test/unit_test.hpp>
#include <server.h>
#include <fstream>
BOOST_AUTO_TEST_CASE(DefaultConstructorTest)
{
    Server server;
    BOOST_CHECK_EQUAL(server.GetIP(), "127.0.0.1");
    BOOST_CHECK_EQUAL(server.GetPort(), 8080);
}

BOOST_AUTO_TEST_CASE(LoadSettingsValidTest)
{
    std::ofstream config_file("valid_config.txt");
    config_file << "IP=192.168.1.1\n";
    config_file << "Port=12345\n";
    config_file.close();

    Server server;
    server.LoadSettings("valid_config.txt");
    BOOST_CHECK_EQUAL(server.GetIP(), "192.168.1.1");
    BOOST_CHECK_EQUAL(server.GetPort(), 12345);
}

BOOST_AUTO_TEST_CASE(LoadSettingsInvalidIPTest)
{
    std::ofstream config_file("invalid_ip_config.txt");
    config_file << "IP=999.999.999.999\n";
    config_file << "Port=12345\n";
    config_file.close();

    Server server;
    server.LoadSettings("invalid_ip_config.txt");
    BOOST_CHECK_EQUAL(server.GetIP(), "127.0.0.1");
    BOOST_CHECK_EQUAL(server.GetPort(), 12345);
}

BOOST_AUTO_TEST_CASE(LoadSettingsInvalidPortTest)
{
    std::ofstream config_file("invalid_port_config.txt");
    config_file << "IP=192.168.1.1\n";
    config_file << "Port=70000\n";
    config_file.close();

    Server server;
    server.LoadSettings("invalid_port_config.txt");
    BOOST_CHECK_EQUAL(server.GetIP(), "192.168.1.1");
    BOOST_CHECK_EQUAL(server.GetPort(), 8080);
}

BOOST_AUTO_TEST_CASE(IsValidIPAddressTest)
{
    Server server;
    BOOST_CHECK(server.IsValidIPAddress("127.0.0.1"));
    BOOST_CHECK(server.IsValidIPAddress("192.168.0.1"));
    BOOST_CHECK(!server.IsValidIPAddress("999.999.999.999"));
    BOOST_CHECK(!server.IsValidIPAddress("abc.def.ghi.jkl"));
}

BOOST_AUTO_TEST_CASE(IsValidPortTest)
{
    Server server;
    BOOST_CHECK(server.IsValidPort(80));
    BOOST_CHECK(server.IsValidPort(8080));
    BOOST_CHECK(server.IsValidPort(65535));
    BOOST_CHECK(!server.IsValidPort(0));
    BOOST_CHECK(!server.IsValidPort(70000));
}

BOOST_AUTO_TEST_CASE(LoadSettingsFileNotFoundTest)
{
    Server server;
    server.LoadSettings("non_existent_config.txt");
    BOOST_CHECK_EQUAL(server.GetIP(), "127.0.0.1");
    BOOST_CHECK_EQUAL(server.GetPort(), 8080);
}
