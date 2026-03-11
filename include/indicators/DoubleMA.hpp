#pragma once

#include "indicators/Indicator.hpp"
#include "indicators/MovingAverage.hpp"

#include <cstddef>
#include <span>
#include <vector>

namespace indicators {

enum class CrossoverType {
    GoldenCross,
    DeathCross
};

struct Crossover {
    std::size_t index;
    CrossoverType type;
};

class DoubleMA : public Indicator {
public:
    DoubleMA(std::size_t fast_window, std::size_t slow_window);

    [[nodiscard]] const char* name() const noexcept override { return "DoubleMA"; }

    [[nodiscard]] constexpr std::size_t fast_window() const noexcept { return fast_.window(); }
    [[nodiscard]] constexpr std::size_t slow_window() const noexcept { return slow_.window(); }

    [[nodiscard]] std::vector<double> compute_fast(std::span<const double> values) const;
    [[nodiscard]] std::vector<double> compute_slow(std::span<const double> values) const;

    [[nodiscard]] std::vector<Crossover> find_crossovers(std::span<const double> values) const;

private:
    MovingAverage fast_;
    MovingAverage slow_;
};

} // namespace indicators

