#pragma once

#include <cstdint>

namespace market_server {
    
    struct Bar {
        std::int64_t timestamp;
        double open;
        double high;
        double low;
        double close;
        std::int64_t volume;

        Bar(std::int64_t ts, double o, double h, double l, double c, std::int64_t v)
            : timestamp(ts), open(o), high(h), low(l), close(c), volume(v) {}
    };

}