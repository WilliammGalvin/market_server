#pragma once

#include <cstdint>
#include <string>

namespace market_server {

    enum class OrderSide {
        Buy, Sell
    };

    enum class OrderType {
        Market, Limit
    };

    struct Order {
        std::uint64_t order_id;
        int client_id;
        
        std::string symbol;
        OrderSide side;
        OrderType type;
        
        double price;
        double quantity;

        std::uint64_t timestamp;
    };

}