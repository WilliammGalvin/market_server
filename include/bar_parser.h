#pragma once

#include "bar.h"
#include "market_date.h"

#include <optional>
#include <vector>
#include <string>
#include <utility>
#include <string_view>
#include <charconv>

#include <iostream>

namespace market_server {

    inline std::optional<MarketDate> parse_market_date(std::string_view data) noexcept {
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

        return MarketDate{
            parts[0], parts[1], parts[2]
        };
    };

    inline std::optional<Bar> parse_csv_to_bar(std::vector<std::string>& data) noexcept {
        constexpr std::size_t bar_val_count = 5;
        if (data.size() != bar_val_count + 1) return std::nullopt;

        auto date_opt = parse_market_date(data[0]);
        if (!date_opt) return std::nullopt;

        double vals[bar_val_count];
        
        for (std::size_t i = 0; i < bar_val_count; ++i) {
            std::string_view value_str = data[i + 1];
            double value = 0;

            if (!value_str.empty() && value_str[0] == '$')
                value_str.remove_prefix(1);

            auto result = std::from_chars(value_str.data(), value_str.data() + value_str.size(), value);
            if (result.ec != std::errc{}) {
                return std::nullopt;
            }
            
            vals[i] = value;
        }

        return Bar{ *date_opt, static_cast<int>(vals[1]), vals[0], vals[2], vals[3], vals[4] };
    }

}