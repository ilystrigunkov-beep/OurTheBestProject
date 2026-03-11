#pragma once

#include <span>

namespace analysis {

struct SummaryStats {
    double mean;
    double stddev;
};

SummaryStats compute_summary(std::span<const double> data);

double mean(std::span<const double> data);

double standard_deviation(std::span<const double> data, double mean_value);

} // namespace analysis

