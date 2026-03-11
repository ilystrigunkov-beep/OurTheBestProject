#pragma once

#include "analysis/Analyzer.hpp"
#include "analysis/Statistics.hpp"

#include <cstddef>
#include <span>
#include <vector>

namespace analysis {

struct Spike {
    std::size_t index;     // Index into the original price series.
    double change_pct;     // Percentage change at this point.
};

class SpikeDetector : public Analyzer {
public:
    explicit SpikeDetector(double threshold_sigma = 2.0)
        : threshold_sigma_(threshold_sigma) {}

    [[nodiscard]] const char* name() const noexcept override { return "SpikeDetector"; }

    [[nodiscard]] double threshold_sigma() const noexcept { return threshold_sigma_; }

    [[nodiscard]] std::vector<Spike> detect(std::span<const double> prices) const;

private:
    double threshold_sigma_;
};

} // namespace analysis

