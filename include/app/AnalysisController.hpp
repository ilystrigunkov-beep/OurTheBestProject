#pragma once

#include "analysis/SpikeDetector.hpp"
#include "core/TimeSeries.hpp"
#include "indicators/DoubleMA.hpp"

#include <QObject>

#include <memory>
#include <string>
#include <vector>
#include <variant>

namespace app {

using SpikeDisplayResult = std::variant<analysis::SpikeList, std::string>;

[[nodiscard]] SpikeDisplayResult to_spike_display_result(const analysis::DetectionOutcome& outcome);

struct AnalysisResult {
    std::unique_ptr<core::TimeSeries> series;
    std::vector<double> closes;
    std::vector<double> sma_fast;
    std::vector<double> sma_slow;
    std::vector<indicators::Crossover> crossovers;
    SpikeDisplayResult spikes;
};

class AnalysisController : public QObject {
    Q_OBJECT
public:
    explicit AnalysisController(QObject* parent = nullptr);

    [[nodiscard]] AnalysisResult load_and_analyze(const std::string& csv_path,
                                                  std::size_t fast_window = 5,
                                                  std::size_t slow_window = 20,
                                                  double spike_sigma = 2.0) const;
};

} // namespace app
