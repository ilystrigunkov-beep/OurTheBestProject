#include "indicators/MovingAverage.hpp"

#include <numeric>
#include <ranges>

namespace indicators {

std::vector<double> MovingAverage::compute(std::span<const double> values) const {
    if (window_ == 0 || values.size() < window_) {
        return {};
    }

    const auto window = window_;
    std::vector<double> result;
    result.reserve(values.size() - window + 1);

    double sum = std::accumulate(values.begin(), values.begin() + static_cast<std::ptrdiff_t>(window), 0.0);
    result.push_back(sum / static_cast<double>(window));

    for (std::size_t i = window; i < values.size(); ++i) {
        sum += values[i] - values[i - window];
        result.push_back(sum / static_cast<double>(window));
    }

    return result;
}

} // namespace indicators

