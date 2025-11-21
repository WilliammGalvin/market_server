#pragma once

#include "market_date.h"

namespace market_server {
    
    struct Bar {
        MarketDate date;
        int volume;
        double close, open, high, low;
    };

}