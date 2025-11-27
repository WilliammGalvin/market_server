#pragma once

#include "order_dispatcher.h"
#include "order.h"

#include <thread>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

namespace market_server {

    class ClientHandler {
    private:
        int socket_;
        OrderDispatcher& dispatcher_;
        std::thread thread_;
        std::atomic<bool> running_{ false };

        void run() {
            running_ = true;

            while (true) {
                char buffer[sizeof(WireOrder)];
                auto n = recv(socket_, buffer, sizeof(buffer), MSG_WAITALL);

                if (n <= 0) {
                    break;
                }

                auto order = parse_order(buffer, n);
                dispatcher_.submit(order);
            }

            close(socket_);
        }

    public:
        ClientHandler(int socket, OrderDispatcher& dispatcher)
            : socket_(socket), dispatcher_(dispatcher) {}
        
        ~ClientHandler() {
            running_ = false;
            if (thread_.joinable())
                thread_.join();
        }

        void start() {
            thread_ = std::thread(&ClientHandler::run, this);
        }

        int socket() const {
            return socket_;
        }
    };

}