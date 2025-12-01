#include "launch_args.h"
#include "thread_queue.h"
#include "producer.h"
#include "acceptor.h"
#include "bar_packet.h"
#include "order_dispatcher.h"
#include "client_handler.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>

#define MARKET_SERVER_SOCKET 5005

void on_bar_ready(const market_server::Bar& bar, market_server::Acceptor& acceptor) {
    // Console output
    std::cout << "Time: " << bar.timestamp << std::endl;
    std::cout << bar.volume << " | $" << bar.open << " $" << bar.close << " $" << bar.high << ", $" << bar.low << "\n" << std::endl;

    // Broadcast
    std::vector<int> clients = acceptor.get_clients();
    market_server::BarPacket packet = bar_to_packet(bar);

    for (int sock : clients) {
        if (send(sock, &packet, sizeof(packet), 0) <= 0) {
            close(sock);
            acceptor.remove_client(sock);
        }
    }
}

int main(int argc, char* argv[]) {
    std::cout << "Launching program..." << std::endl;

    auto argsOpt = market_server::LaunchArgs::parse(argc, argv);    
    if (!argsOpt) {
        throw std::runtime_error("Could not parse launch arguments.");
    }

    market_server::LaunchArgs args = *argsOpt;
    market_server::ThreadQueue<market_server::Bar> bar_queue;
    market_server::OrderDispatcher order_dispatcher{};

    // Acceptor thread to accept incoming client connections
    signal(SIGPIPE, SIG_IGN);
    market_server::Acceptor acceptor{ MARKET_SERVER_SOCKET, order_dispatcher };
    acceptor.start();

    // Order thread to process incoming orders
    std::thread order_thread(&market_server::OrderDispatcher::processing_loop, &order_dispatcher);
    order_thread.detach();

    // Producer thread to read and parse market data
    market_server::Producer producer{ args, bar_queue };
    producer.start();

    // Consumer thread (main) to read data
    while (true) { 
        market_server::Bar bar = bar_queue.pop();
        on_bar_ready(bar, acceptor);

        std::this_thread::sleep_for(std::chrono::milliseconds(
            static_cast<std::int64_t>(args.day_length_ms))
        );
    }

    acceptor.stop();
    producer.join();
    return 0;
}