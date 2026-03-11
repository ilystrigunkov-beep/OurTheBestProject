#pragma once

#include "core/TimeSeries.hpp"

#include <memory>
#include <string>

namespace core {

class CsvLoader {
public:
    // Load a CSV file with columns: timestamp,open,high,low,close[,volume]
    // Timestamp is treated as a Unix timestamp in seconds.
    static std::unique_ptr<TimeSeries> load(const std::string& path, char delimiter = ',');
};

} // namespace core

