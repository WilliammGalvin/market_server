#pragma once

#include "order.h"

#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>

namespace market_server {

    class OrderDispatcher {
    private:
        std::queue<Order> order_queue_;
        std::mutex mutex_;
        std::condition_variable cond_var_;

    public:
        void submit(const Order& order) {
            std::lock_guard<std::mutex> lock(mutex_);
            order_queue_.push(order);
            cond_var_.notify_one();
        }

        void processing_loop() {
            while (true) {
                std::unique_lock<std::mutex> lock(mutex_);
                cond_var_.wait(lock, [&]{
                    return !order_queue_.empty();
                });

                Order order = order_queue_.front();
                order_queue_.pop();

                // TODO: Implement order handling
                if (order.side == OrderSide::Buy) {
                    std::cout << "Recieved buy order from client " << order.client_id << ".\n";
                } else {
                    std::cout << "Recieved sell order from client " << order.client_id << ".\n";
                }
            }
        }
    };

}