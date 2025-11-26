#pragma once

#include "bar.h"

#include <optional>
#include <vector>
#include <string>
#include <string_view>
#include <charconv>
#include <cstdint>
#include <iostream>
#include <chrono>

namespace market_server {

    inline std::optional<std::int64_t> parse_market_timestamp(std::string_view data) noexcept {
        std::string parts[3];
        std::size_t idx = 0;
        std::size_t start = 0;

        for (std::size_t end = 0; end <= data.size(); ++end) {
            if (end == data.size() || data[end] == '/') {
                if (idx >= 3) 
                    return std::nullopt;

                parts[idx++] = data.substr(start, end - start);
                start = end + 1;
            }
        }

        if (idx != 3) {
            return std::nullopt;
        }

        for (const auto& p : parts) {
            if (p.empty()) return std::nullopt;
        }

        int month, day, year;
        auto to_int = [&](std::string_view s, int& out) {
            auto res = std::from_chars(s.data(), s.data() + s.size(), out);
            return res.ec == std::errc{};
        };

        if (!to_int(parts[0], month))
            return std::nullopt;

        if (!to_int(parts[1], day))
            return std::nullopt;

        if (!to_int(parts[2], year))
            return std::nullopt;

        if (month < 1 || month > 12)
            return std::nullopt;

        if (day < 1 || day > 31)
            return std::nullopt;

        const std::chrono::year_month_day ymd{
            std::chrono::year{ year },
            std::chrono::month{ static_cast<unsigned>(month) },
            std::chrono::day{ static_cast<unsigned>(day) }
        };

        if (!ymd.ok())
            return std::nullopt;

        std::chrono::sys_days days{ ymd }; 
        auto ts = std::chrono::duration_cast<std::chrono::seconds>(days.time_since_epoch()).count();
        return static_cast<std::int64_t>(ts);
    };

    inline std::optional<Bar> parse_csv_to_bar(std::vector<std::string>& data) noexcept {
        constexpr std::size_t bar_val_count = 5;
        if (data.size() != bar_val_count + 1) return std::nullopt;

        auto timestamp_opt = parse_market_timestamp(data[0]);
        if (!timestamp_opt) return std::nullopt;

        double vals[bar_val_count];
        
        for (std::size_t i = 0; i < bar_val_count; ++i) {
            std::string_view value_str = data[i + 1];

            if (!value_str.empty() && value_str[0] == '$')
                value_str.remove_prefix(1);

            char* endptr = nullptr;
            std::string temp{ value_str };

            double value = std::strtod(temp.c_str(), &endptr);
            if (endptr == temp.c_str()) return std::nullopt;
            
            vals[i] = value;
        }

        auto [close, volume, open, high, low] = vals;
        return Bar(*timestamp_opt, open, high, low, close, volume);
    }

}