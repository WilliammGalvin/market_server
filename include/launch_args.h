#pragma once

#include <string>
#include <optional>
#include <sstream>
#include <iostream>

namespace market_server {

    struct LaunchArgs {
        std::string data_path;
        float time_scale; // Scale of 2 = 3.25h market day

        static std::optional<LaunchArgs> parse(int argc, char* argv[]) {
            if (argc < 3) {
                std::cerr << "Usage: " << argv[0] << " <data-path> <time-scale>\n";
                return std::nullopt;
            }

            LaunchArgs args;
            args.data_path = argv[1];

            std::istringstream ss(argv[2]);
            if (!(ss >> args.time_scale)) {
                std::cerr << "Invalid time_scale: " << argv[2] << "\n";
                return std::nullopt;
            }

            return args;
        }
    };

}