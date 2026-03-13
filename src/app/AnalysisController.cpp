#include "app/AnalysisController.hpp"

#include "core/CsvLoader.hpp"

#include <span>

namespace app {

SpikeDisplayResult to_spike_display_result(const analysis::DetectionOutcome& outcome) {
    if (outcome.has_value()) {
        return *outcome;
    }
    return outcome.error();
}

AnalysisController::AnalysisController(QObject* parent)
    : QObject(parent) {}

AnalysisResult AnalysisController::load_and_analyze(const std::string& csv_path,
                                                    std::size_t fast_window,
                                                    std::size_t slow_window,
                                                    double spike_sigma) const {
    AnalysisResult out;
    out.series = core::CsvLoader::load(csv_path);
    out.closes = out.series->closes();

    std::span<const double> closes_span(out.closes.data(), out.closes.size());

    indicators::DoubleMA dma(fast_window, slow_window);
    out.sma_fast = dma.compute_fast(closes_span);
    out.sma_slow = dma.compute_slow(closes_span);
    out.crossovers = dma.find_crossovers(closes_span);

    analysis::SpikeDetector detector(spike_sigma);
    out.spikes = to_spike_display_result(detector.detect(closes_span));

    return out;
}

} // namespace app
