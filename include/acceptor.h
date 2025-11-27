#pragma once

#include "order_dispatcher.h"
#include "client_handler.h"

#include <vector>
#include <mutex>
#include <thread>
#include <netinet/in.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <memory>
#include <atomic>

namespace market_server {

    class Acceptor {
    private:
        int port_;
        OrderDispatcher& dispatcher_;
        int listen_socket_;
        std::atomic<bool> is_running_;
        std::vector<int> clients_;
        std::mutex clients_mutex_;
        std::thread thread_;
        std::vector<std::unique_ptr<ClientHandler>> handlers_;

        void on_client_connect(int socket) {
            std::cout << "Client connected on socket " << socket << "\n\n";
        }
        
        void on_client_disconnect(int socket) {
            std::cout << "Client disconnected on socket " << socket << "\n\n";
        }

        void on_server_setup() {
            if (!is_running_) return;
            std::cout << "Server running on port " << port_ << "\n\n";
        }
            
        void run() {
            while (is_running_) {
                int client_socket = accept(listen_socket_, nullptr, nullptr);
                if (client_socket < 0) {
                    if (is_running_) continue;
                    else break;
                }

                on_client_connect(client_socket);

                auto handler = std::make_unique<ClientHandler>(client_socket, dispatcher_);
                handler->start();

                {
                    std::lock_guard<std::mutex> lock(clients_mutex_);
                    clients_.push_back(client_socket);
                    handlers_.push_back(std::move(handler));
                }
            }
        }

    public:
        Acceptor(int port, OrderDispatcher& dispatcher)
            : port_(port), dispatcher_(dispatcher), is_running_(false) {

            listen_socket_ = socket(AF_INET, SOCK_STREAM, 0);
            sockaddr_in addr{};
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = INADDR_ANY;
            addr.sin_port = htons(port_);

            bind(listen_socket_, (struct sockaddr*)&addr, sizeof(addr));
            listen(listen_socket_, 10); 
        }

        ~Acceptor() {
            stop();
            close(listen_socket_);
        }

        void start() {
            is_running_ = true;
            on_server_setup();
            thread_ = std::thread(&Acceptor::run, this);
        }
        
        void stop() {
            is_running_ = false;
            if (thread_.joinable())
                thread_.join();
        }

        void remove_client(int sock) {
            std::lock_guard<std::mutex> lock{ clients_mutex_ };
            
            clients_.erase(
                std::remove(clients_.begin(), clients_.end(), sock),
                clients_.end()
            );

            handlers_.erase(
                std::remove_if(
                    handlers_.begin(),
                    handlers_.end(),
                    [&](const std::unique_ptr<ClientHandler>& h) {
                        return h->socket() == sock;
                    }
                ),
                handlers_.end()
            );
            
            on_client_disconnect(sock);
        }

        std::vector<int> get_clients() {
            std::lock_guard<std::mutex> lock(clients_mutex_);
            return clients_;
        }
    };

}