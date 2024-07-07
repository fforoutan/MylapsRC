#include "include/client.h"

#include <QFileDialog>
#include <QSettings>
#include <memory>

Client::Client(QObject* parent) : QObject(parent), socket_(std::make_unique<QTcpSocket>(this)) {
    loadSettings();
}

Client::~Client()
{
    socket_->close();
}

void Client::loadSettings()
{
    QSettings settings("resources/config.ini", QSettings::IniFormat);
    ip_ = settings.value("Network/IP", "127.0.0.1").toString();
    QRegExp ip_regex("\\b(?:\\d{1,3}\\.){3}\\d{1,3}\\b");
    if (!ip_regex.exactMatch(ip_)) {
        qDebug() << "Invalid IP address in configuration file. Using default IP: 127.0.0.1";
        ip_ = "127.0.0.1";
    }
    bool port_ok;
    int temp_port = settings.value("Network/Port", 8088).toInt(&port_ok);
    if (!port_ok || temp_port <= 0 || temp_port > 65535) {
        qDebug() << "Invalid port number in configuration file. Using default port: 8088";
        port_ = 8088;
    } else {
        port_ = temp_port;
    }

    qDebug() << "Loaded settings: IP =" << ip_ << ", Port =" << port_;
}
