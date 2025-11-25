#pragma once

#include <thread>

#include "launch_args.h"
#include "csv_reader.h"
#include "bar_parser.h"
#include "thread_queue.h"
#include "bar.h"

namespace market_server {

    class Producer {
    private:
        LaunchArgs& args_;
        ThreadQueue<Bar>& queue_;
        std::thread thread_;

        void run() {
            market_server::CSVReader reader{ args_.data_path.c_str() };

            if (!reader.open_file()) {
                throw std::runtime_error("Could not find data file.");
            }
            
            // CSV Line = Date, Close/Last, Volume, Open, High, Low
            while (auto vals = reader.read_csv_line()) {
                auto bar_opt = parse_csv_to_bar(*vals);
                if (!bar_opt) {
                    throw std::runtime_error("Could not parse CSV to bar.");
                }

                queue_.push(*bar_opt);
            }
        }

    public:
        Producer(LaunchArgs& args, ThreadQueue<Bar>& queue)
            : args_(args), queue_(queue) {}

        void start() {
            thread_ = std::thread(&Producer::run, this);
        }

        void join() {
            if (thread_.joinable())
                thread_.join();            
        }
    };

}