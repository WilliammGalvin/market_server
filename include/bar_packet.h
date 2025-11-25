#pragma once

#include "bar.h"

#include <cstdint>

namespace market_server{

    #pragma pack(push, 1)
    struct BarPacket {
        std::int64_t timestamp;
        double open;
        double close;
        double high;
        double low;
        std::int64_t volume;
    };
    #pragma pack(pop)

    inline BarPacket bar_to_packet(const Bar& bar) {
        BarPacket packet{};
        packet.timestamp = bar.timestamp;
        packet.open = bar.open;
        packet.close = bar.close;
        packet.high = bar.high;
        packet.low = bar.low;
        packet.volume = bar.volume;
    
        return packet;
    }

}