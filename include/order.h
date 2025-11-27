#pragma once

#include <cstdint>
#include <string>
#include <cassert>
#include <cstring>

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

    #pragma pack(push, 1)
    struct WireOrder {
        std::uint64_t order_id;
        std::uint32_t client_id;

        char symbol[8];
        OrderSide side;
        OrderType type;
        
        double price;
        double quantity;

        std::uint64_t timestamp;
    };
    #pragma pack(pop)

    inline Order parse_order(char* buffer, ssize_t size) {
        assert(size == sizeof(WireOrder));
        const WireOrder* wire_order = reinterpret_cast<const WireOrder*>(buffer);

        Order order;
        order.order_id = wire_order->order_id;
        order.client_id = wire_order->client_id;
        order.symbol = std::string(wire_order->symbol, strnlen(wire_order->symbol, 8));
        order.side = wire_order->side;
        order.type = wire_order->type;
        order.price = wire_order->price;
        order.quantity = wire_order->quantity;
        order.timestamp = wire_order->timestamp;

        return order;
    }

}