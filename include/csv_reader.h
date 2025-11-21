#pragma once

#include "reader.h"

#include <vector>
#include <string>
#include <optional>
#include <sstream>

namespace market_server {

    class CSVReader final : public Reader<std::string> {
    private:
        std::string trim(const std::string& s) {
            auto start = s.begin();
            while (start != s.end() && std::isspace(*start)) {
                start++;
            }

            auto end = s.end();
            do {
                end--;
            } while (std::distance(start, end) > 0 && std::isspace(*end));

            return std::string(start, end + 1);
        }

    public:
        CSVReader(const char* file_path)
            : Reader(file_path) {}

        ~CSVReader() = default;

        std::optional<std::vector<std::string>> read_csv_line() {
            auto line_opt = read_line();
            if (!line_opt) {
                return std::nullopt;
            }

            std::vector<std::string> res{};
            std::stringstream ss{ *line_opt };
            std::string cell;

            while (std::getline(ss, cell, ',')) {
                res.push_back(trim(cell));
            }

            return res;
        }
    };

}