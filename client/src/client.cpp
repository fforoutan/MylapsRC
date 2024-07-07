#include "include/client.h"

#include <QFileDialog>
#include <QMessageBox>
#include <fstream>
#include <sstream>
#include <QTime>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>
#include <QSettings>

Client::Client(QObject* parent) : QObject(parent), socket_(std::make_unique<QTcpSocket>(this)) {
    loadSettings();
    connect(socket_.get(), &QTcpSocket::readyRead, this, &Client::readMessage);
    connect(socket_.get(), qOverload<QAbstractSocket::SocketError>(&QTcpSocket::errorOccurred), this, &Client::handleError);

    if (!socket_) {
        socket_->connectToHost(ip_, port_);
    }
}

Client::~Client()
{
    socket_->close();
}

void Client::readMessage() {
    QByteArray data = socket_->readAll();
    QString response = QString::fromUtf8(data);
    qDebug() << "respond:" << response;
    emit responseReceived(response);
}

void Client::sendCSVData() {
    QString filename = QFileDialog::getOpenFileName(nullptr, "Open CSV File", "", "CSV Files (*.csv)");
    qDebug() << "file name:" << filename;
    if (!filename.isEmpty()) {
        readCSVAndSend(filename);
    }
}

void Client::readCSVAndSend(const QString& filename) {
    emit responseReceived("");
    if (!socket_) {
        socket_->connectToHost(ip_, port_);
    }
    std::ifstream file(filename.toStdString());
    if (!file.is_open()) {
        QMessageBox::critical(nullptr, "Error", "Failed to open the CSV file.");
        return;
    }

    std::string line;
    std::ostringstream oss;
    std::getline(file, line);
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string driver_number, time;

        if (std::getline(ss, driver_number, ',') && std::getline(ss, time, ',')) {
            oss << driver_number << "," << time << "\n";
        }
    }

    std::string message = oss.str();
    qDebug() << "message" << message.c_str();
    if (socket_->state() == QAbstractSocket::ConnectedState) {
        socket_->write(message.c_str(), message.size());
    } else {
        socket_->connectToHost(ip_, port_);
        socket_->write(message.c_str(), message.size());
    }

    file.close();
}

void Client::handleError(QAbstractSocket::SocketError socket_error) {
    Q_UNUSED(socket_error);
    qDebug() << "Socket error:" << socket_->errorString();
    emit responseReceived("Socket error: " + socket_->errorString());
}

void Client::loadSettings()
{
    QSettings settings("config.ini", QSettings::IniFormat);

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
