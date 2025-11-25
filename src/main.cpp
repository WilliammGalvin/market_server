#include "launch_args.h"
#include "thread_queue.h"
#include "producer.h"

#include <iostream>
#include <thread>
#include <chrono>

void on_bar_rec(const market_server::Bar& bar) {
    std::cout << "Time: " << bar.timestamp << "\n";
    std::cout << bar.volume << " | $" << bar.open << " $" << bar.close << " $" << bar.high << ", $" << bar.low << "\n\n";
}

int main(int argc, char* argv[]) {
    auto argsOpt = market_server::LaunchArgs::parse(argc, argv);    
    if (!argsOpt) {
        throw std::runtime_error("Could not parse launch arguments.");
    }

    market_server::LaunchArgs args = *argsOpt;
    market_server::ThreadQueue<market_server::Bar> bar_queue;

    // Producer thread to read and parse market data
    market_server::Producer producer{ args, bar_queue };
    producer.start();

    // Consumer thread (main) to read data
    while (true) { 
        market_server::Bar bar = bar_queue.pop();
        on_bar_rec(bar);

        std::this_thread::sleep_for(std::chrono::milliseconds(
            static_cast<std::int64_t>(args.day_length_ms))
        );
    }

    producer.join();
    return 0;
}