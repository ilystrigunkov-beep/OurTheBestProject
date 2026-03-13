#pragma once
#include <memory>
#include <span>
namespace analysis {
struct SummaryStats {
    double mean;
    double stddev;
};
double mean(std::span<const double> data);
double standard_deviation(std::span<const double> data, double mean_value);
[[nodiscard]] std::shared_ptr<SummaryStats> compute_summary(std::span<const double> data);
}
