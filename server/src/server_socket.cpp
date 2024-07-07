#include "server_socket.h"
#include <iostream>
#include <thread>

server_socket::server_socket(int port)
    : port_(port), listen_socket_(INVALID_SOCKET), running_(true) {}

server_socket::~server_socket() {
    running_ = false;
    closesocket(listen_socket_);
    WSACleanup();
    condition_.notify_all();
    if (accept_thread_.joinable()) {
        accept_thread_.join();
    }
    for (auto& worker : worker_threads_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

void server_socket::Run() {
    SetupServer();
    accept_thread_ = std::thread(&server_socket::AcceptConnections, this);
    for (size_t i = 0; i < std::thread::hardware_concurrency(); ++i) {
        worker_threads_.emplace_back(&server_socket::WorkerThread, this);
    }
}

void server_socket::SetupServer() {
    WSADATA wsa_data;
    int result;

    struct addrinfo* addr_result = nullptr;
    struct addrinfo hints;

    result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (result != 0) {
        std::cerr << "WSAStartup failed with error: " << result << std::endl;
        exit(EXIT_FAILURE);
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    std::string port_str = std::to_string(port_);
    result = getaddrinfo(nullptr, port_str.c_str(), &hints, &addr_result);
    if (result != 0) {
        std::cerr << "getaddrinfo failed with error: " << result << std::endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    listen_socket_ = socket(addr_result->ai_family, addr_result->ai_socktype, addr_result->ai_protocol);
    if (listen_socket_ == INVALID_SOCKET) {
        std::cerr << "Error at socket(): " << WSAGetLastError() << std::endl;
        freeaddrinfo(addr_result);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    result = bind(listen_socket_, addr_result->ai_addr, static_cast<int>(addr_result->ai_addrlen));
    if (result == SOCKET_ERROR) {
        std::cerr << "bind failed with error: " << WSAGetLastError() << std::endl;
        freeaddrinfo(addr_result);
        closesocket(listen_socket_);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(addr_result);

    if (listen(listen_socket_, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed with error: " << WSAGetLastError() << std::endl;
        closesocket(listen_socket_);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
}

void server_socket::AcceptConnections() {
    while (running_) {
        SOCKET client_socket = accept(listen_socket_, nullptr, nullptr);
        if (client_socket == INVALID_SOCKET) {
            if (running_) {
                std::cerr << "accept failed: " << WSAGetLastError() << std::endl;
            }
            continue;
        }
        std::cout << "Client connected: " << client_socket << std::endl;
        std::thread(&server_socket::HandleClient, this, client_socket).detach();
    }
}

void server_socket::WorkerThread() {
    while (running_) {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        condition_.wait(lock, [this] { return !message_queue_.empty() || !running_; });
        while (!message_queue_.empty()) {
            auto [client_socket, message] = std::move(message_queue_.front());
            message_queue_.pop();
            lock.unlock();
            if (message_handler_) {
                message_handler_(message, client_socket);
            }
            lock.lock();
        }
    }
}

void server_socket::HandleClient(SOCKET client_socket) {
    char recv_buf[512];
    int recv_buf_len = 512;
    int recv_result;

    while ((recv_result = recv(client_socket, recv_buf, recv_buf_len, 0)) > 0) {
        std::string message(recv_buf, recv_result);
        {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            message_queue_.emplace(client_socket, message);
        }
        condition_.notify_one();
    }

    if (recv_result == 0) {
        std::cout << "Connection closing..." << std::endl;
    } else {
        std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
    }

    closesocket(client_socket);
}

void server_socket::SetMessageHandler(const std::function<void(const std::string&, SOCKET)>& handler) {
    message_handler_ = handler;
}
