#pragma once

#include "indicators/Indicator.hpp"

#include <cstddef>
#include <span>
#include <vector>

namespace indicators {

class MovingAverage : public Indicator {
public:
    explicit constexpr MovingAverage(std::size_t window) : window_(window) {}

    [[nodiscard]] const char* name() const noexcept override { return "SMA"; }

    [[nodiscard]] constexpr std::size_t window() const noexcept { return window_; }

    [[nodiscard]] std::vector<double> compute(std::span<const double> values) const;

private:
    std::size_t window_;
};

} // namespace indicators

