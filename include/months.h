#pragma once

#include <string>

namespace market_server {

    inline std::string date_str_to_str(const std::string& date) {
        if (date == "1") return "JAN";
        if (date == "2") return "FEB";
        if (date == "3") return "MAR";
        if (date == "4") return "APR";
        if (date == "5") return "MAY";
        if (date == "6") return "JUN";
        if (date == "7") return "JUL";
        if (date == "8") return "AUG";
        if (date == "9") return "SEP";
        if (date == "10") return "OCT";
        if (date == "11") return "NOV";
        if (date == "12") return "DEC";

        return "UNKNOWN";
    }

}
