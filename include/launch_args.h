#pragma once

#include <string>
#include <optional>
#include <sstream>
#include <iostream>

namespace market_server {

    struct LaunchArgs {
        std::string data_path;
        float day_length_ms;

        static std::optional<LaunchArgs> parse(int argc, char* argv[]) {
            if (argc < 3) {
                std::cerr << "Usage: " << argv[0] << " <data-path> <day_length_ms>\n";
                return std::nullopt;
            }

            LaunchArgs args;
            args.data_path = argv[1];

            std::istringstream ss(argv[2]);
            if (!(ss >> args.day_length_ms)) {
                std::cerr << "Invalid day_time_ms: " << argv[2] << "\n";
                return std::nullopt;
            }

            return args;
        }
    };

}