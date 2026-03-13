#pragma once
#include "analysis/Analyzer.hpp"
#include "analysis/Statistics.hpp"
#include <cstddef>
#include <expected>
#include <memory>
#include <span>
#include <string>
#include <vector>
namespace analysis {
struct Spike {
    std::size_t index;
    double change_pct;
};
using SpikeList = std::vector<Spike>;
using DetectionOutcome = std::expected<SpikeList, std::string>;
class SpikeDetector : public Analyzer {
public:
    explicit SpikeDetector(double threshold_sigma = 2.0) : threshold_sigma_(threshold_sigma) {}
    [[nodiscard]] const char* name() const noexcept override { return "SpikeDetector"; }
    [[nodiscard]] std::shared_ptr<Analyzer> clone() const override {
        return std::make_shared<SpikeDetector>(*this);
    }
    [[nodiscard]] DetectionOutcome detect(std::span<const double> prices) const;
    [[nodiscard]] double threshold_sigma() const noexcept { return threshold_sigma_; }
private:
    double threshold_sigma_;
};
}
