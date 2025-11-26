#pragma once

#include "order_dispatcher.h"

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

        void run() {
            while (true) {
                char buffer[1024];
                auto n = recv(socket_, buffer, sizeof(buffer), 0);

                if (n <= 0) {
                    break;
                }

                std::ptrdiff_t bytes_received = n;
                auto order = parse_order(buffer, bytes_received);
                dispatcher_.submit(order);
            }

            close(socket_);
        }

    public:
        ClientHandler(int socket, OrderDispatcher& dispatcher)
            : socket_(socket), dispatcher_(dispatcher) {

                thread_ = std::thread(&ClientHandler::run, this);
            } 
    };

}