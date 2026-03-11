#include "core/TimeSeries.hpp"

#include <stdexcept>

namespace core {

TimeSeries::TimeSeries(std::vector<Candle> candles)
    : candles_(std::move(candles)) {}

void TimeSeries::add_candle(const Candle& candle) {
    candles_.push_back(candle);
}

std::size_t TimeSeries::size() const noexcept {
    return candles_.size();
}

const Candle& TimeSeries::at(std::size_t index) const {
    if (index >= candles_.size()) {
        throw std::out_of_range("TimeSeries::at - index out of range");
    }
    return candles_[index];
}

std::span<const Candle> TimeSeries::candles() const noexcept {
    return std::span<const Candle>(candles_.data(), candles_.size());
}

std::vector<double> TimeSeries::closes() const {
    std::vector<double> result;
    result.reserve(candles_.size());
    for (const auto& c : candles_) {
        result.push_back(c.close());
    }
    return result;
}

} // namespace core

