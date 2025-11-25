#include "launch_args.h"
#include "csv_reader.h"
#include "bar_parser.h"
#include "months.h"

#include <iostream>
#include <thread>
#include <chrono>

int main(int argc, char* argv[]) {
    auto argsOpt = market_server::LaunchArgs::parse(argc, argv);    
    if (!argsOpt) {
        throw std::runtime_error("Could not parse launch arguments.");
    }

    market_server::LaunchArgs args = *argsOpt;
    market_server::CSVReader reader{ args.data_path.c_str() };    

    if (!reader.open_file()) {
        throw std::runtime_error("Could not find data file.");
    }
    
    // CSV Line = Date, Close/Last, Volume, Open, High, Low
    while (auto vals = reader.read_csv_line()) {
        auto bar_opt = market_server::parse_csv_to_bar(*vals);
        if (!bar_opt) {
            throw std::runtime_error("Could not parse CSV to bar.");
        }

        const market_server::Bar& bar = *bar_opt;
        std::cout << "Time: " << bar.timestamp << "\n";
        std::cout << bar.volume << " | $" << bar.open << " $" << bar.close << " $" << bar.high << ", $" << bar.low << "\n\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(
            static_cast<std::int64_t>(args.day_length_ms))
        );
    }

    return 0;
}