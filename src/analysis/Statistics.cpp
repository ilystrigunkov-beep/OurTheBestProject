#include "analysis/Statistics.hpp"

#include <cmath>
#include <numeric>
#include <vector>

namespace analysis {

double mean(std::span<const double> data) {
    if (data.empty()) {
        return 0.0;
    }
    const double sum = std::reduce(data.begin(), data.end(), 0.0);
    return sum / static_cast<double>(data.size());
}

double standard_deviation(std::span<const double> data, double mean_value) {
    if (data.size() <= 1) {
        return 0.0;
    }

    double sum_sq = 0.0;
    for (double v : data) {
        const double diff = v - mean_value;
        sum_sq += diff * diff;
    }
    const double variance = sum_sq / static_cast<double>(data.size() - 1);
    return std::sqrt(variance);
}

SummaryStats compute_summary(std::span<const double> data) {
    const double m = mean(data);
    const double s = standard_deviation(data, m);
    return SummaryStats{m, s};
}

} // namespace analysis

