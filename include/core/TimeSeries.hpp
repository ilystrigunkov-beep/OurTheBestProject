#pragma once

#include "core/Candle.hpp"

#include <cstddef>
#include <span>
#include <vector>

namespace core {

class TimeSeries {
public:
    TimeSeries() = default;
    explicit TimeSeries(std::vector<Candle> candles);

    ~TimeSeries() = default;
    TimeSeries(const TimeSeries&) = default;
    TimeSeries(TimeSeries&&) noexcept = default;
    TimeSeries& operator=(const TimeSeries&) = default;
    TimeSeries& operator=(TimeSeries&&) noexcept = default;

    void add_candle(const Candle& candle);

    [[nodiscard]] std::size_t size() const noexcept;
    [[nodiscard]] const Candle& at(std::size_t index) const;

    [[nodiscard]] std::span<const Candle> candles() const noexcept;

    // Convenience helper for indicators: extract closing prices.
    [[nodiscard]] std::vector<double> closes() const;

private:
    std::vector<Candle> candles_;
};

} // namespace core

