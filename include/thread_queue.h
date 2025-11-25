#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <utility>

namespace market_server {

    template <typename T>
    class ThreadQueue {
    private:
        std::queue<T> data_;
        std::mutex lock_;
        std::condition_variable cond_var_full_;
        std::condition_variable cond_var_empty_;
        std::size_t capacity_{ 10 };
        
    public:
        ThreadQueue() = default;
        ThreadQueue(std::size_t capacity) : capacity_(capacity) {}

        template <typename U>
        void push(U&& v) {
            std::unique_lock<std::mutex> lock(lock_);
            cond_var_full_.wait(lock, [&] {
                return data_.size() < capacity_;
            });

            data_.emplace(std::forward<U>(v));
            cond_var_empty_.notify_one();
        }

        T pop() {
            std::unique_lock<std::mutex> lock(lock_);
            cond_var_empty_.wait(lock, [&]{
                return !data_.empty();
            });

            T v = std::move(data_.front());
            data_.pop();
            cond_var_full_.notify_one();

            return v;
        }
    };

}